@echo off
cd ..
set BOOST_PATH=A:/BuildTools/Boost
em++ examples/nsdl_test.cpp -I include -I dependencies/glm -I dependencies/NumCpp/include -I %BOOST_PATH%/include -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_GFX=2 -s USE_SDL_TTF=2 -s USE_SDL_MIXER=2 -s USE_SDL_NET=2 -s LEGACY_GL_EMULATION=1 -std=c++17 -s SINGLE_FILE --shell-file examples/assets/emscripten_template.html -s SDL2_IMAGE_FORMATS=['bmp','png','jpg'] -s TOTAL_MEMORY=512MB --embed-file examples/assets -o examples/bin/index.html
cd examples
