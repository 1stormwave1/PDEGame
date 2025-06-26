

using UnrealBuildTool;

public class HorrorGame : ModuleRules
{
	public HorrorGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		OptimizeCode = CodeOptimization.Never;
		bUseRTTI = true;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "UMG", "Slate", "SlateCore", "PCG", "AIModule" });
	}
}
