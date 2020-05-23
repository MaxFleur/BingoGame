# BingoGame

This project creates a basic BingoGame with 25 fields, a joker and some entrys. The entrys are randomized at the begin of the game and via clicking a restart "button".\
The game is written just for fun and for developing programming skills. It's written in C++ and uses LibCinder[1]. 
Since I'm relatively unexperienced with programming, the code seen in this project has a lot of potential for improvements. Feel free to comment on possible optimizations!

# Setup Instructions

The BingoGame works under Windows and Linux. Instructions for both systems follow.

## Windows

1. Clone and setup LibCinder. It is strongly recommended to do this via git. Further setup instructions for Cinder and Windows can be found under [2].
2. After setting up LibCinder, clone the BingoGame repository into the Cinder folder.
3. The BingoGame should now be in the directory ..\Cinder\BingoGame. Open the .sln under "vc2017" and compile the project.

## Linux

1. Clone and setup LibCinder for Linux. It is strongly recommended to do this via git. Further setup instructions for Cinder and Linux can be found under [3].
2. After setting up LibCinder, clone the BingoGame repository into the Cinder folder.
3. The BingoGame should now be in the directory ../Cinder/BingoGame. 
a) Go to /proj/cmake.
b) In your terminal, type mkdir build && cd build.
c) Now type cmake .. and then make. The project now should be created.

# References

[1] https://libcinder.org/

[2] https://libcinder.org/docs/guides/git/index.html

[3] https://www.libcinder.org/docs/guides/linux-notes/ubuntu.html

The texture for the board was obtained via:
https://www.flickr.com/photos/webtreatsetc/4727355663

The winning sound was obtained via:
http://soundbible.com/1823-Winning-Triumphal-Fanfare.html

Licensed under CC 3.0:
https://creativecommons.org/licenses/by/3.0/
