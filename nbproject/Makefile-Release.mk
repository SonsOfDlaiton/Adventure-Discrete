#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/Classes/AL.o \
	${OBJECTDIR}/Classes/Blocks.o \
	${OBJECTDIR}/Classes/Bullet.o \
	${OBJECTDIR}/Classes/Enemy.o \
	${OBJECTDIR}/Classes/Entity.o \
	${OBJECTDIR}/Classes/GL.o \
	${OBJECTDIR}/Classes/Map.o \
	${OBJECTDIR}/Classes/Mechanics.o \
	${OBJECTDIR}/Classes/Player.o \
	${OBJECTDIR}/Classes/Scenes.o \
	${OBJECTDIR}/Classes/Tutorials.o \
	${OBJECTDIR}/Classes/Util.o \
	${OBJECTDIR}/Classes/mapEdit.o \
	${OBJECTDIR}/Classes/powerUp.o \
	${OBJECTDIR}/Libs/GL.CallBacks.o \
	${OBJECTDIR}/Libs/Globals.o \
	${OBJECTDIR}/Libs/loadFiles.o \
	${OBJECTDIR}/Libs/soil/SOIL.o \
	${OBJECTDIR}/Libs/soil/image_DXT.o \
	${OBJECTDIR}/Libs/soil/image_helper.o \
	${OBJECTDIR}/Libs/soil/stb_image_aug.o \
	${OBJECTDIR}/TCG.main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/the.com-game

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/the.com-game: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/the.com-game ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/Classes/AL.o: Classes/AL.cpp 
	${MKDIR} -p ${OBJECTDIR}/Classes
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Classes/AL.o Classes/AL.cpp

${OBJECTDIR}/Classes/Blocks.o: Classes/Blocks.cpp 
	${MKDIR} -p ${OBJECTDIR}/Classes
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Classes/Blocks.o Classes/Blocks.cpp

${OBJECTDIR}/Classes/Bullet.o: Classes/Bullet.cpp 
	${MKDIR} -p ${OBJECTDIR}/Classes
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Classes/Bullet.o Classes/Bullet.cpp

${OBJECTDIR}/Classes/Enemy.o: Classes/Enemy.cpp 
	${MKDIR} -p ${OBJECTDIR}/Classes
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Classes/Enemy.o Classes/Enemy.cpp

${OBJECTDIR}/Classes/Entity.o: Classes/Entity.cpp 
	${MKDIR} -p ${OBJECTDIR}/Classes
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Classes/Entity.o Classes/Entity.cpp

${OBJECTDIR}/Classes/GL.o: Classes/GL.cpp 
	${MKDIR} -p ${OBJECTDIR}/Classes
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Classes/GL.o Classes/GL.cpp

${OBJECTDIR}/Classes/Map.o: Classes/Map.cpp 
	${MKDIR} -p ${OBJECTDIR}/Classes
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Classes/Map.o Classes/Map.cpp

${OBJECTDIR}/Classes/Mechanics.o: Classes/Mechanics.cpp 
	${MKDIR} -p ${OBJECTDIR}/Classes
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Classes/Mechanics.o Classes/Mechanics.cpp

${OBJECTDIR}/Classes/Player.o: Classes/Player.cpp 
	${MKDIR} -p ${OBJECTDIR}/Classes
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Classes/Player.o Classes/Player.cpp

${OBJECTDIR}/Classes/Scenes.o: Classes/Scenes.cpp 
	${MKDIR} -p ${OBJECTDIR}/Classes
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Classes/Scenes.o Classes/Scenes.cpp

${OBJECTDIR}/Classes/Tutorials.o: Classes/Tutorials.cpp 
	${MKDIR} -p ${OBJECTDIR}/Classes
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Classes/Tutorials.o Classes/Tutorials.cpp

${OBJECTDIR}/Classes/Util.o: Classes/Util.cpp 
	${MKDIR} -p ${OBJECTDIR}/Classes
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Classes/Util.o Classes/Util.cpp

${OBJECTDIR}/Classes/mapEdit.o: Classes/mapEdit.cpp 
	${MKDIR} -p ${OBJECTDIR}/Classes
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Classes/mapEdit.o Classes/mapEdit.cpp

${OBJECTDIR}/Classes/powerUp.o: Classes/powerUp.cpp 
	${MKDIR} -p ${OBJECTDIR}/Classes
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Classes/powerUp.o Classes/powerUp.cpp

${OBJECTDIR}/Libs/GL.CallBacks.o: Libs/GL.CallBacks.cpp 
	${MKDIR} -p ${OBJECTDIR}/Libs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Libs/GL.CallBacks.o Libs/GL.CallBacks.cpp

${OBJECTDIR}/Libs/Globals.o: Libs/Globals.cpp 
	${MKDIR} -p ${OBJECTDIR}/Libs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Libs/Globals.o Libs/Globals.cpp

${OBJECTDIR}/Libs/loadFiles.o: Libs/loadFiles.cpp 
	${MKDIR} -p ${OBJECTDIR}/Libs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Libs/loadFiles.o Libs/loadFiles.cpp

${OBJECTDIR}/Libs/soil/SOIL.o: Libs/soil/SOIL.c 
	${MKDIR} -p ${OBJECTDIR}/Libs/soil
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Libs/soil/SOIL.o Libs/soil/SOIL.c

${OBJECTDIR}/Libs/soil/image_DXT.o: Libs/soil/image_DXT.c 
	${MKDIR} -p ${OBJECTDIR}/Libs/soil
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Libs/soil/image_DXT.o Libs/soil/image_DXT.c

${OBJECTDIR}/Libs/soil/image_helper.o: Libs/soil/image_helper.c 
	${MKDIR} -p ${OBJECTDIR}/Libs/soil
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Libs/soil/image_helper.o Libs/soil/image_helper.c

${OBJECTDIR}/Libs/soil/stb_image_aug.o: Libs/soil/stb_image_aug.c 
	${MKDIR} -p ${OBJECTDIR}/Libs/soil
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Libs/soil/stb_image_aug.o Libs/soil/stb_image_aug.c

${OBJECTDIR}/TCG.main.o: TCG.main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/TCG.main.o TCG.main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/the.com-game

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
