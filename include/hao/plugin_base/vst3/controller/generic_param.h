// Copyright(c) 2021 Hansen Audio.

#pragma once

#include "hao/audio_modules/types.h"
#include "hao/plugin_base/core/types.h"
#include "pluginterfaces/vst/ivsteditcontroller.h"
#include "pluginterfaces/vst/vsttypes.h"
#include "public.sdk/source/vst/vstparameters.h"
#include <functional>
#include <memory>

namespace hao::audio_modules {
enum class ModuleTags;
struct ParamInfo;
} // namespace hao::audio_modules

namespace hao::plugin_base::vst3 {

//-----------------------------------------------------------------------------
class GenericParam : public Steinberg::Vst::Parameter
{
public:
    //-------------------------------------------------------------------------
    static GenericParam* create(Steinberg::Vst::UnitID unitID,
                                audio_modules::ModuleTags nodeTag,
                                audio_modules::ParamInfo const& info);

    using ParamCreatedFunc = std::function<bool(GenericParam*)>;

    static void createParameters(Steinberg::Vst::UnitID unitID,
                                 audio_modules::ModuleTags nodeTag,
                                 ParamCreatedFunc func);

    static void createParameters(entity_component_def const& def,
                                 ParamCreatedFunc func);

    GenericParam();
    GenericParam(const Steinberg::Vst::ParameterInfo&);
    GenericParam(
        const Steinberg::Vst::TChar* title,
        Steinberg::Vst::ParamID tag,
        const Steinberg::Vst::TChar* units                = nullptr,
        Steinberg::Vst::ParamValue defaultValueNormalized = 0.,
        Steinberg::int32 stepCount                        = 0,
        Steinberg::int32 flags = Steinberg::Vst::ParameterInfo::kCanAutomate,
        Steinberg::Vst::UnitID unitID           = Steinberg::Vst::kRootUnitId,
        const Steinberg::Vst::TChar* shortTitle = nullptr);

    ~GenericParam() override;

    /** Converts a normalized value to a string. */
    void toString(Steinberg::Vst::ParamValue valueNormalized,
                  Steinberg::Vst::String128 string) const override;
    /** Converts a string to a normalized value. */
    bool fromString(const Steinberg::Vst::TChar* string,
                    Steinberg::Vst::ParamValue& valueNormalized) const override;

    /** Converts a normalized value to plain value (e.g. 0.5 to 10000.0Hz). */
    Steinberg::Vst::ParamValue
    toPlain(Steinberg::Vst::ParamValue valueNormalized) const override;
    /** Converts a plain value to a normalized value (e.g. 10000 to 0.5). */
    Steinberg::Vst::ParamValue
    toNormalized(Steinberg::Vst::ParamValue plainValue) const override;

    void setConvertTag(audio_modules::ModuleTags nodeTag,
                       audio_modules::mut_tag_type convertTag);

    OBJ_METHODS(GenericParam, Steinberg::Vst::Parameter)
    //------------------------------------------------------------------------
protected:
    Steinberg::Vst::ParameterInfo info;
    Steinberg::Vst::ParamValue valueNormalized;
    Steinberg::int32 precision;

private:
    audio_modules::ModuleTags nodeTag;
    audio_modules::mut_tag_type convertTag;
};

//-----------------------------------------------------------------------------
} // namespace hao::plugin_base::vst3