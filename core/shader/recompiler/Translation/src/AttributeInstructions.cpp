#include "Translation/AttributeInstructions.hpp"
#include "Translation/TranslationContext.hpp"
#include "Recompiler.hpp"
#include <algorithm>
#include <array>
#include <bit>
#include <stdexcept>

namespace ShaderRecompiler {

namespace {

ExportTargetKind exportTargetKindFromTarget(std::uint32_t target, std::uint32_t& index) {
    index = 0u;
    switch (target) {
        case 0x08u: return ExportTargetKind::MrtZ;
        case 0x09u: return ExportTargetKind::Null;
        case 0x14u: return ExportTargetKind::Primitive;
        default: break;
    }
    if (target <= 0x07u) {
        index = target;
        return ExportTargetKind::Mrt;
    }
    if (target >= 0x0cu && target <= 0x0fu) {
        index = target - 0x0cu;
        return ExportTargetKind::Position;
    }
    if (target >= 0x20u && target <= 0x3fu) {
        index = target - 0x20u;
        return ExportTargetKind::Parameter;
    }
    return ExportTargetKind::Unknown;
}

}

void TranslateAttributeInstruction(IrBuilder& builder, const RdnaInstruction& instruction, const TranslateOptions& options) {
    throw std::runtime_error("TranslateAttributeInstruction not implemented");
}

ExportFlags TranslationContext::addExportInfo(const RdnaInstruction& inst) {
    ExportInfo info;
    info.kind = exportTargetKindFromTarget(inst.exportTarget, info.index);
    info.target = inst.exportTarget;
    info.en = inst.exportEnableMask;
    info.done = inst.exportIsLast;
    info.compr = inst.exportIsCompressed;
    info.vm = inst.exportValidMask;
    const std::uint32_t index = static_cast<std::uint32_t>(program.Metadata().exportInfo.size());
    program.Metadata().exportInfo.push_back(info);
    return ExportFlags{index, inst.programCounter};
}

IrValue& TranslationContext::barycentricP1(const RdnaInstruction& inst, std::uint32_t attr, std::uint32_t chan) {
    auto& delta = ir.Emit(IrOpcode::GetInterpolationParameter, IrType::U32, {&ir.Constant(attr), &ir.Constant(chan), &ir.Constant(0u)});
    auto& origin = ir.Emit(IrOpcode::GetInterpolationParameter, IrType::U32, {&ir.Constant(attr), &ir.Constant(chan), &ir.Constant(2u)});
    auto& product = ir.Emit(IrOpcode::FPMul32, IrType::F32, {&ir.BitCastF32(delta), readOperand(inst.source0, IrType::F32)});
    return ir.Emit(IrOpcode::FPAdd32, IrType::F32, {&product, &ir.BitCastF32(origin)});
}

IrU32 TranslationContext::packF16FromF32(IrValue& value) {
    const IrF16 half(ir.Emit(IrOpcode::ConvertF16F32, IrType::F16, {&value}));
    const IrU16 bits(ir.Emit(IrOpcode::BitCastU16F16, IrType::U16, {&half.Value()}));
    return IrU32(ir.Emit(IrOpcode::ConvertU32U16, IrType::U32, {&bits.Value()}));
}

void TranslationContext::vInterpP1F32(const RdnaInstruction& inst) {
    if (!fragmentShaderBarycentricEnabled) return;
    writeOperand(inst.destination, &barycentricP1(inst, inst.source1.value, inst.source2.value));
}

void TranslationContext::vInterpP1llF16(const RdnaInstruction& inst) {
    if (!fragmentShaderBarycentricEnabled) return;
    const std::uint32_t attr = inst.source1.value;
    const std::uint32_t chan = inst.source2.value & ~1u;
    const IrU32 low = packF16FromF32(barycentricP1(inst, attr, chan));
    const IrU32 high = packF16FromF32(barycentricP1(inst, attr, chan | 1u));
    const IrU32 packed(ir.BitwiseOr(low.Value(), ir.ShiftLeftLogical(high.Value(), ir.Constant(16u))));
    writeOperand(inst.destination, &packed.Value());
}

void TranslationContext::vInterpP1lvF16(const RdnaInstruction& inst) {
    if (!fragmentShaderBarycentricEnabled) return;
    const IrU32 previous = readU32(sourceAt(inst, 3u));
    const IrU32 low(ir.BitwiseAnd(previous.Value(), ir.Constant(0xffffu)));
    const IrU32 high = packF16FromF32(barycentricP1(inst, inst.source1.value, inst.source2.value | 1u));
    const IrU32 packed(ir.BitwiseOr(low.Value(), ir.ShiftLeftLogical(high.Value(), ir.Constant(16u))));
    writeOperand(inst.destination, &packed.Value());
}

void TranslationContext::vInterpP2F16(const RdnaInstruction& inst) {
    if (fragmentShaderBarycentricEnabled) {
        const RdnaOperand accumulator = inst.sourceCount >= 4u ? sourceAt(inst, 3u) : inst.destination;
        auto& delta = ir.Emit(IrOpcode::GetInterpolationParameter, IrType::U32, {&ir.Constant(inst.source1.value), &ir.Constant(inst.source2.value), &ir.Constant(1u)});
        auto& product = ir.Emit(IrOpcode::FPMul32, IrType::F32, {&ir.BitCastF32(delta), readOperand(inst.source0, IrType::F32)});
        auto& result = ir.Emit(IrOpcode::FPAdd32, IrType::F32, {&product, readOperand(accumulator, IrType::F32)});
        const IrU32 packed = packF16FromF32(result);
        writeOperand(inst.destination, &packed.Value());
        return;
    }
    IrValue& value = ir.Emit(IrOpcode::GetAttribute, IrType::U32, {&ir.Constant(inst.source1.value), &ir.Constant(inst.source2.value)});
    const IrU32 packed = packF16FromF32(ir.BitCastF32(value));
    writeOperand(inst.destination, &packed.Value());
}

void TranslationContext::vInterpP2F32(const RdnaInstruction& inst) {
    if (fragmentShaderBarycentricEnabled) {
        auto& delta = ir.Emit(IrOpcode::GetInterpolationParameter, IrType::U32, {&ir.Constant(inst.source1.value), &ir.Constant(inst.source2.value), &ir.Constant(1u)});
        auto& product = ir.Emit(IrOpcode::FPMul32, IrType::F32, {&ir.BitCastF32(delta), readOperand(inst.source0, IrType::F32)});
        auto& result = ir.Emit(IrOpcode::FPAdd32, IrType::F32, {&product, readOperand(inst.destination, IrType::F32)});
        writeOperand(inst.destination, &result);
        return;
    }
    if (pixelInput != nullptr && inst.source0.kind == RdnaOperandKind::VectorRegister && inst.source1.value < 32u) {
        const auto readsPair = [&](PixelInput input) {
            const auto base = pixelInput->psInputVgpr[static_cast<std::size_t>(input)];
            return base != ShaderPixelInputInfo::NoPixelInputVgpr && inst.source0.reg == base + 1u;
        };
        const auto bit = 1u << inst.source1.value;
        if (readsPair(PixelInput::LinearCenter) || readsPair(PixelInput::LinearCentroid)) {
            program.Metadata().pixelLinearInputs |= bit;
        } else if (readsPair(PixelInput::PerspectiveCenter) || readsPair(PixelInput::PerspectiveCentroid)) {
            program.Metadata().pixelPerspectiveInputs |= bit;
        }
    }
    IrValue& value = ir.Emit(IrOpcode::GetAttribute, IrType::U32, {&ir.Constant(inst.source1.value), &ir.Constant(inst.source2.value)});
    writeOperand(inst.destination, &value);
}

void TranslationContext::vInterpMovF32(const RdnaInstruction& inst) {
    if (inst.source0.value >= 3u) {
        throw std::runtime_error("v_interp_mov_f32 mode is reserved");
    }
    IrValue& value = ir.Emit(IrOpcode::GetInterpolationParameter, IrType::U32, {&ir.Constant(inst.source1.value), &ir.Constant(inst.source2.value), &ir.Constant(inst.source0.value)});
    writeOperand(inst.destination, &value);
}

void TranslationContext::eXP(const RdnaInstruction& inst) {
    std::uint32_t index = 0u;
    if (exportTargetKindFromTarget(inst.exportTarget, index) == ExportTargetKind::Unknown) {
        throw std::runtime_error("unsupported EXP target");
    }
    std::array<IrValue*, 4> components{&ir.Constant(0u), &ir.Constant(0u), &ir.Constant(0u), &ir.Constant(0u)};
    const std::uint32_t sourceCount = std::min(inst.sourceCount, 4u);
    for (std::uint32_t source = 0u; source < sourceCount; ++source) {
        components[source] = &readRawU32(plainOperand(sourceAt(inst, source))).Value();
    }
    IrValue& data = ir.Emit(IrOpcode::CompositeConstructU32x4, IrType::U32x4, {components[0], components[1], components[2], components[3]});
    IrValue& exec = ir.GetExec();
    (void)ir.Emit(IrOpcode::SetAttribute, IrType::Void, {&data, &exec}, addExportInfo(inst));
}

bool TranslationContext::emitInterpolation(const RdnaInstruction& inst) {
    if ((inst.op == RdnaOpcode::VInterpP1F32 || inst.op == RdnaOpcode::VInterpP2F32 || inst.op == RdnaOpcode::VInterpP1llF16 || inst.op == RdnaOpcode::VInterpP1lvF16 || inst.op == RdnaOpcode::VInterpP2F16) && pixelInput != nullptr && pixelInput->InputIsCustom(inst.source1.value)) {
        throw std::runtime_error("pixel input " + std::to_string(inst.source1.value) + " passes its vertices through unchanged but is read with v_interp_p1/p2");
    }
    switch (inst.op) {
        case RdnaOpcode::VInterpP1F32:
            vInterpP1F32(inst);
            return true;
        case RdnaOpcode::VInterpP2F32:
            vInterpP2F32(inst);
            return true;
        case RdnaOpcode::VInterpMovF32:
            vInterpMovF32(inst);
            return true;
        case RdnaOpcode::VInterpP1llF16:
            vInterpP1llF16(inst);
            return true;
        case RdnaOpcode::VInterpP1lvF16:
            vInterpP1lvF16(inst);
            return true;
        case RdnaOpcode::VInterpP2F16:
            vInterpP2F16(inst);
            return true;
        default:
            return false;
    }
}

void TranslationContext::TranslateEmbeddedFetch(const RdnaInstruction& instruction, std::uint32_t attribute, std::uint32_t components) {
    if (!instruction.formatted || instruction.typed || components == 0u || components > 4u) throw std::runtime_error("invalid prepared vertex fetch");
    for (std::uint32_t component = 0; component < components; ++component) {
        auto& value = ir.Emit(IrOpcode::GetAttribute, IrType::U32, {&ir.Constant(attribute), &ir.Constant(component)});
        value.SetFlags<std::uint32_t>(1u);
        writeOperand(offsetOperand(instruction.destination, component), &value);
    }
}

void TranslateAttributeInstruction(TranslationContext& context, const RdnaInstruction& instruction) {
    throw std::runtime_error("TranslateAttributeInstruction not implemented");
}

}
