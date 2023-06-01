#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <chrono>
#include <unordered_map>
#include <optional>
#include <thread>
#include <tuple>

#include <nlohmann/json.hpp>

#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_opengl.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/hash.hpp>

#include <cryptopp/cryptlib.h>
#include <cryptopp/secblock.h>
#include <cryptopp/modes.h>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/osrng.h>
#include <cryptopp/rsa.h>
#include <cryptopp/hex.h>

#include <openssl/aes.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>

#include <zlib.h>

#include <curl/curl.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif