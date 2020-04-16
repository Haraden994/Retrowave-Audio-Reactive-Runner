##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=AudioVisualizer
ConfigurationName      :=Debug
WorkspacePath          :=C:/Users/night/Desktop/AudioVisualizer
ProjectPath            :=C:/Users/night/Desktop/AudioVisualizer/lectures_final/lecture06
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=night
Date                   :=14/04/2020
CodeLitePath           :="C:/Program Files/CodeLite"
LinkerName             :=C:/MinGW/bin/g++.exe
SharedObjectLinkerName :=C:/MinGW/bin/g++.exe -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="AudioVisualizer.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/MinGW/bin/windres.exe
LinkOptions            :=  -static-libgcc -static-libstdc++
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../../include $(IncludeSwitch)../../include/bullet $(IncludeSwitch)../../include/irrKlang $(IncludeSwitch)../../include/aubio $(IncludeSwitch)../../include/imgui 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)glfw3 $(LibrarySwitch)assimp $(LibrarySwitch)BulletDynamics $(LibrarySwitch)BulletCollision $(LibrarySwitch)Bullet3Dynamics $(LibrarySwitch)Bullet3Collision $(LibrarySwitch)LinearMath $(LibrarySwitch)irrKlang $(LibrarySwitch)aubio 
ArLibs                 :=  "glfw3" "assimp" "BulletDynamics" "BulletCollision" "Bullet3Dynamics" "Bullet3Collision" "LinearMath" "irrKlang" "aubio" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)../../libs/win 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/MinGW/bin/ar.exe rcu
CXX      := C:/MinGW/bin/g++.exe
CC       := C:/MinGW/bin/gcc.exe
CXXFLAGS :=  -g -O0 -Wall -std=c++0x $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := C:/MinGW/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
Objects0=$(IntermediateDirectory)/up_up_include_glad_glad.c$(ObjectSuffix) $(IntermediateDirectory)/AudioVisualizer.cpp$(ObjectSuffix) $(IntermediateDirectory)/imgui.cpp$(ObjectSuffix) $(IntermediateDirectory)/imgui_draw.cpp$(ObjectSuffix) $(IntermediateDirectory)/imgui_impl_glfw.cpp$(ObjectSuffix) $(IntermediateDirectory)/ImGuiFileDialog.cpp$(ObjectSuffix) $(IntermediateDirectory)/imgui_demo.cpp$(ObjectSuffix) $(IntermediateDirectory)/imgui_widgets.cpp$(ObjectSuffix) $(IntermediateDirectory)/imgui_impl_opengl3.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

PostBuild:
	@echo Executing Post Build commands ...
	copy ..\..\libs\win\*.dll .\Debug
	copy *.vert Debug
	copy *.frag Debug
	@echo Done

MakeIntermediateDirs:
	@$(MakeDirCommand) "./Debug"


$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./Debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/up_up_include_glad_glad.c$(ObjectSuffix): ../../include/glad/glad.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_include_glad_glad.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_include_glad_glad.c$(DependSuffix) -MM ../../include/glad/glad.c
	$(CC) $(SourceSwitch) "C:/Users/night/Desktop/AudioVisualizer/include/glad/glad.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_include_glad_glad.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_include_glad_glad.c$(PreprocessSuffix): ../../include/glad/glad.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_include_glad_glad.c$(PreprocessSuffix) ../../include/glad/glad.c

$(IntermediateDirectory)/AudioVisualizer.cpp$(ObjectSuffix): AudioVisualizer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/AudioVisualizer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/AudioVisualizer.cpp$(DependSuffix) -MM AudioVisualizer.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/night/Desktop/AudioVisualizer/lectures_final/lecture06/AudioVisualizer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/AudioVisualizer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/AudioVisualizer.cpp$(PreprocessSuffix): AudioVisualizer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/AudioVisualizer.cpp$(PreprocessSuffix) AudioVisualizer.cpp

$(IntermediateDirectory)/imgui.cpp$(ObjectSuffix): imgui.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/imgui.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/imgui.cpp$(DependSuffix) -MM imgui.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/night/Desktop/AudioVisualizer/lectures_final/lecture06/imgui.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/imgui.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/imgui.cpp$(PreprocessSuffix): imgui.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/imgui.cpp$(PreprocessSuffix) imgui.cpp

$(IntermediateDirectory)/imgui_draw.cpp$(ObjectSuffix): imgui_draw.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/imgui_draw.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/imgui_draw.cpp$(DependSuffix) -MM imgui_draw.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/night/Desktop/AudioVisualizer/lectures_final/lecture06/imgui_draw.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/imgui_draw.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/imgui_draw.cpp$(PreprocessSuffix): imgui_draw.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/imgui_draw.cpp$(PreprocessSuffix) imgui_draw.cpp

$(IntermediateDirectory)/imgui_impl_glfw.cpp$(ObjectSuffix): imgui_impl_glfw.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/imgui_impl_glfw.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/imgui_impl_glfw.cpp$(DependSuffix) -MM imgui_impl_glfw.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/night/Desktop/AudioVisualizer/lectures_final/lecture06/imgui_impl_glfw.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/imgui_impl_glfw.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/imgui_impl_glfw.cpp$(PreprocessSuffix): imgui_impl_glfw.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/imgui_impl_glfw.cpp$(PreprocessSuffix) imgui_impl_glfw.cpp

$(IntermediateDirectory)/ImGuiFileDialog.cpp$(ObjectSuffix): ImGuiFileDialog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ImGuiFileDialog.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ImGuiFileDialog.cpp$(DependSuffix) -MM ImGuiFileDialog.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/night/Desktop/AudioVisualizer/lectures_final/lecture06/ImGuiFileDialog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ImGuiFileDialog.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ImGuiFileDialog.cpp$(PreprocessSuffix): ImGuiFileDialog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ImGuiFileDialog.cpp$(PreprocessSuffix) ImGuiFileDialog.cpp

$(IntermediateDirectory)/imgui_demo.cpp$(ObjectSuffix): imgui_demo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/imgui_demo.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/imgui_demo.cpp$(DependSuffix) -MM imgui_demo.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/night/Desktop/AudioVisualizer/lectures_final/lecture06/imgui_demo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/imgui_demo.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/imgui_demo.cpp$(PreprocessSuffix): imgui_demo.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/imgui_demo.cpp$(PreprocessSuffix) imgui_demo.cpp

$(IntermediateDirectory)/imgui_widgets.cpp$(ObjectSuffix): imgui_widgets.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/imgui_widgets.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/imgui_widgets.cpp$(DependSuffix) -MM imgui_widgets.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/night/Desktop/AudioVisualizer/lectures_final/lecture06/imgui_widgets.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/imgui_widgets.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/imgui_widgets.cpp$(PreprocessSuffix): imgui_widgets.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/imgui_widgets.cpp$(PreprocessSuffix) imgui_widgets.cpp

$(IntermediateDirectory)/imgui_impl_opengl3.cpp$(ObjectSuffix): imgui_impl_opengl3.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/imgui_impl_opengl3.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/imgui_impl_opengl3.cpp$(DependSuffix) -MM imgui_impl_opengl3.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/night/Desktop/AudioVisualizer/lectures_final/lecture06/imgui_impl_opengl3.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/imgui_impl_opengl3.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/imgui_impl_opengl3.cpp$(PreprocessSuffix): imgui_impl_opengl3.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/imgui_impl_opengl3.cpp$(PreprocessSuffix) imgui_impl_opengl3.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


