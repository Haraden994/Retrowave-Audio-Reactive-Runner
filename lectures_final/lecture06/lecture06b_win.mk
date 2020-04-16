##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=lecture06b_win
ConfigurationName      :=Debug
WorkspacePath          :="C:/Users/Super/OneDrive/Desktop/UNIEcc/RTGP/RTGP_LAB/1819 - AUBIO - Copia"
ProjectPath            :="C:/Users/Super/OneDrive/Desktop/UNIEcc/RTGP/RTGP_LAB/1819 - AUBIO - Copia/lectures_final/lecture06"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Super
Date                   :=11/08/2019
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
ObjectsFileList        :="lecture06b_win.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/MinGW/bin/windres.exe
LinkOptions            :=  -static-libgcc -static-libstdc++
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../../include $(IncludeSwitch)../../include/bullet $(IncludeSwitch)../../include/irrKlang $(IncludeSwitch)../../include/aubio 
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
Objects0=$(IntermediateDirectory)/lecture06b.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_include_glad_glad.c$(ObjectSuffix) 



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
$(IntermediateDirectory)/lecture06b.cpp$(ObjectSuffix): lecture06b.cpp $(IntermediateDirectory)/lecture06b.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Super/OneDrive/Desktop/UNIEcc/RTGP/RTGP_LAB/1819 - AUBIO - Copia/lectures_final/lecture06/lecture06b.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lecture06b.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lecture06b.cpp$(DependSuffix): lecture06b.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lecture06b.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/lecture06b.cpp$(DependSuffix) -MM lecture06b.cpp

$(IntermediateDirectory)/lecture06b.cpp$(PreprocessSuffix): lecture06b.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lecture06b.cpp$(PreprocessSuffix) lecture06b.cpp

$(IntermediateDirectory)/up_up_include_glad_glad.c$(ObjectSuffix): ../../include/glad/glad.c $(IntermediateDirectory)/up_up_include_glad_glad.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/Super/OneDrive/Desktop/UNIEcc/RTGP/RTGP_LAB/1819 - AUBIO - Copia/include/glad/glad.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_include_glad_glad.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_include_glad_glad.c$(DependSuffix): ../../include/glad/glad.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_include_glad_glad.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_include_glad_glad.c$(DependSuffix) -MM ../../include/glad/glad.c

$(IntermediateDirectory)/up_up_include_glad_glad.c$(PreprocessSuffix): ../../include/glad/glad.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_include_glad_glad.c$(PreprocessSuffix) ../../include/glad/glad.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


