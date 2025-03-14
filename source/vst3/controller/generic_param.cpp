// Copyright(c) 2021 Hansen Audio.

#include "ha/plugin_base/vst3/controller/generic_param.h"
#include "hao/audio_modules/audio_modules.h"
#include "ha/plugin_base/core/types.h"
#include "pluginterfaces/vst/ivsteditcontroller.h"
#include "public.sdk/source/vst/utility/stringconvert.h"
#include "vst3/common/param_id.h"

namespace ha::plugin_base::vst3 {

//-----------------------------------------------------------------------------
GenericParam* GenericParam::create(Steinberg::Vst::UnitID unitId,
                                   audio_modules::ModuleTags nodeTag,
                                   audio_modules::ParamInfo const& param_info)
{
    auto const converter =
        audio_modules::ModuleFactoryImpl::convert_funcs(nodeTag).at(
            param_info.convert_tag);

    Steinberg::Vst::ParameterInfo info;
    info.defaultNormalizedValue = param_info.default_normalised;
    info.flags                  = 0;
    if (param_info.is_read_only)
        info.flags |= Steinberg::Vst::ParameterInfo::kIsReadOnly;
    else
        info.flags |= Steinberg::Vst::ParameterInfo::kCanAutomate;
    info.id        = build_param_id(unitId, param_info.param_tag);
    info.stepCount = converter.num_steps();
    if (info.stepCount > 1)
        info.flags |= Steinberg::Vst::ParameterInfo::kIsList;
    info.unitId = unitId;
    VST3::StringConvert::convert(std::string(param_info.units), info.units);
    VST3::StringConvert::convert(std::string(param_info.name), info.title);

    GenericParam* genericParam = new GenericParam{info};
    genericParam->setConvertTag(nodeTag, param_info.convert_tag);
    return genericParam;
}

//-----------------------------------------------------------------------------
GenericParam::GenericParam() {}

//-----------------------------------------------------------------------------
GenericParam::GenericParam(const Steinberg::Vst::ParameterInfo& info)
: Parameter(info)
{
}

//-----------------------------------------------------------------------------
GenericParam::GenericParam(const Steinberg::Vst::TChar* title,
                           Steinberg::Vst::ParamID tag,
                           const Steinberg::Vst::TChar* units,
                           Steinberg::Vst::ParamValue defaultValueNormalized,
                           Steinberg::int32 stepCount,
                           Steinberg::int32 flags,
                           Steinberg::Vst::UnitID unitID,
                           const Steinberg::Vst::TChar* shortTitle)
: Parameter(title,
            tag,
            units,
            defaultValueNormalized,
            stepCount,
            flags,
            unitID,
            shortTitle)
{
}

//-----------------------------------------------------------------------------
GenericParam::~GenericParam() {}

//-----------------------------------------------------------------------------
void GenericParam::toString(Steinberg::Vst::ParamValue valueNormalized,
                            Steinberg::Vst::String128 string) const
{
    using amf = audio_modules::ModuleFactoryImpl;

    auto const converter = amf::convert_funcs(nodeTag).at(convertTag);
    if (!converter.to_physical || !converter.to_string)
        return;

    const auto phys    = converter.to_physical(valueNormalized);
    const auto std_str = converter.to_string(phys);
    VST3::StringConvert::convert(std_str, string);
}

//-----------------------------------------------------------------------------
bool GenericParam::fromString(const Steinberg::Vst::TChar* string,
                              Steinberg::Vst::ParamValue& valueNormalized) const
{
    using amf = audio_modules::ModuleFactoryImpl;

    auto const converter = amf::convert_funcs(nodeTag).at(convertTag);
    if (!converter.from_string || !converter.to_normalised)
        return false;

    auto const std_string = VST3::StringConvert::convert(string);
    auto const phys       = converter.from_string(std_string);
    auto const norm       = converter.to_normalised(phys);

    valueNormalized = norm;

    return true;
}

//-----------------------------------------------------------------------------
Steinberg::Vst::ParamValue
GenericParam::toPlain(Steinberg::Vst::ParamValue valueNormalized) const
{
    using amf = audio_modules::ModuleFactoryImpl;

    auto const converter = amf::convert_funcs(nodeTag).at(convertTag);
    if (!converter.to_physical)
        return 0;

    return converter.to_physical(valueNormalized);
}

//-----------------------------------------------------------------------------
Steinberg::Vst::ParamValue
GenericParam::toNormalized(Steinberg::Vst::ParamValue plainValue) const
{
    using amf = audio_modules::ModuleFactoryImpl;

    auto const converter = amf::convert_funcs(nodeTag).at(convertTag);
    if (!converter.to_normalised)
        return 0;

    return converter.to_normalised(plainValue);
}

//-----------------------------------------------------------------------------
void GenericParam::createParameters(Steinberg::Vst::UnitID unitId,
                                    audio_modules::ModuleTags audioNodeTag,
                                    ParamCreatedFunc func)
{
    using amf = audio_modules::ModuleFactoryImpl;

    for (audio_modules::ParamInfo const& param_info :
         amf::param_infos(audioNodeTag))
    {
        GenericParam* genericParam =
            GenericParam::create(unitId, audioNodeTag, param_info);

        if (func)
            func(genericParam); // takes ownership
    }
}

//-----------------------------------------------------------------------------
void GenericParam::setConvertTag(audio_modules::ModuleTags nodeTag,
                                 audio_modules::mut_tag_type convertTag)
{
    this->nodeTag    = nodeTag;
    this->convertTag = convertTag;
}

//-----------------------------------------------------------------------------
void GenericParam::createParameters(entity_component_def const& def,
                                    ParamCreatedFunc func)
{
    for (const auto& element : def)
    {
        Steinberg::Vst::UnitID const unit_id             = element.first;
        audio_modules::ModuleTags const audio_module_tag = element.second;
        GenericParam::createParameters(unit_id, audio_module_tag, func);
    }
}
//-----------------------------------------------------------------------------
} // namespace ha::plugin_base::vst3