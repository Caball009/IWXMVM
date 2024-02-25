#pragma once

namespace IWXMVM::Types
{
    enum class KeyframeablePropertyType
    {
        CampathCamera,
        SunLightColor,
        SunLightBrightness,
        SunLightPitch,
        SunLightYaw,
        FilmtweakBrightness,
        FilmtweakContrast,
        FilmtweakDesaturation,
        FilmtweakTintLight,
        FilmtweakTintDark,
        DepthOfFieldFarBlur,
        DepthOfFieldFarStart,
        DepthOfFieldFarEnd,
        DepthOfFieldNearBlur,
        DepthOfFieldNearStart,
        DepthOfFieldNearEnd,
        DepthOfFieldBias
    };

    enum class KeyframeValueType
    {
        FloatingPoint,
        Vector3,
        CameraData
    };

    struct KeyframeableProperty
    {
        Types::KeyframeablePropertyType type;
        std::string_view name;
        KeyframeValueType valueType;

        KeyframeableProperty(Types::KeyframeablePropertyType type, std::string_view name, KeyframeValueType valueType)
            : type(type), name(name), valueType(valueType)
        {
        }

        bool operator<(const KeyframeableProperty& other) const
        {
            return name < other.name;
        }

       public:
        int32_t GetValueCount() const
        {
            // I dont really like the way this is done, I just cant really think of a better solution right now
            switch (valueType)
            {
                case Types::KeyframeValueType::FloatingPoint:
                    return 1;
                case Types::KeyframeValueType::CameraData:
                    return 7;
                case Types::KeyframeValueType::Vector3:
                    return 3;
                default:
                    throw std::runtime_error("Not implemented");
            }
        };
    };
}