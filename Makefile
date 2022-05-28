#OBJS specifies which files to compile as part of the project
OBJS1 = DOS.cpp networking.cpp
OBJS2 = server.cpp card_handling.cpp
#CC specifies which compiler we're using
CC1 = clang++
CC2 = clang++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS1 = -w
COMPILER_FLAGS2 = -w

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS1 = -I/Library/Frameworks/SDL2.framework/Headers -I/Library/Frameworks/SDL2_ttf.framework/Headers -F/Library/Frameworks/ -framework SDL2 -F/Library/Frameworks/ -framework SDL2_ttf
LINKER_FLAGS2 =

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME1 = DOS_GAME
OBJ_NAME2 = DOS_SERVER

#This is the target that compiles our executable
all: $(OBJS1) $(OBJS2)

DOS : $(OBJS1)
	$(CC1) $(OBJS1) -o $(OBJ_NAME1) $(COMPILER_FLAGS1) $(LINKER_FLAGS1)

DOS_SERVER : $(OBJS2)
	$(CC2) $(OBJS2) -o $(OBJ_NAME2) $(COMPILER_FLAGS2) $(LINKER_FLAGS2)







#clang++ new.cpp -o new -I/Library/Frameworks/SDL2.framework/Headers -I/Library/Frameworks/SDL2_ttf.framework/Headers -F/Library/Frameworks/ -framework SDL2 -F/Library/Frameworks/ -framework SDL2_ttf
