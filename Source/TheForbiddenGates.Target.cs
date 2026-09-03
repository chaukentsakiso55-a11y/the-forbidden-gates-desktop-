using UnrealBuildTool;
using System.Collections.Generic;

public class TheForbiddenGatesTarget : TargetRules
{
    public TheForbiddenGatesTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;
        ExtraModuleNames.Add("TheForbiddenGates");
    }
}
