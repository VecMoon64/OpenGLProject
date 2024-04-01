#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "portaudio/portaudio.h"
#include <math.h>
#include "moonyplatform.h"
#include "curl/curl.h"

using namespace std;

#define SAMPLE_RATE  (44100)
#define FRAMES_PER_BUFFER (64)
#define M_PI  (3.14159265)

std::string httpResult = ""; // HTTP isteği sonucunu saklayacak

size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void PerformHttpRequest() { // HTTP isteği yapma fonksiyonu
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://88.231.77.212:80/mysite/?userId=1");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            httpResult = readBuffer; // HTTP isteğinin sonucunu global değişkende sakla
        }
        curl_easy_cleanup(curl);
    }
}

typedef struct
{
    double phase;
    double phaseIncrement;
} paTestData;

static int paCallback(const void* inputBuffer, void* outputBuffer,
                      unsigned long framesPerBuffer,
                      const PaStreamCallbackTimeInfo* timeInfo,
                      PaStreamCallbackFlags statusFlags,
                      void* userData)
{
    paTestData* data = (paTestData*)userData;
    float* out = (float*)outputBuffer;
    unsigned int i;
    (void)inputBuffer; /* Prevent unused variable warning. */

    for (i = 0; i < framesPerBuffer; i++)
    {
        *out++ = (float)sin(data->phase) * 0.5f; /* Left channel */
        *out++ = (float)sin(data->phase) * 0.5f; /* Right channel */
        data->phase += data->phaseIncrement;
        if (data->phase >= (M_PI * 2))
        {
            data->phase -= (M_PI * 2);
        }
    }

    return paContinue;
}

int main()
{
    // PortAudio initialization
    PaStream* stream;
    PaError err;
    paTestData data;

    print_message("Merhaba");

    err = Pa_Initialize();
    if (err != paNoError)
    {
        cout << "PortAudio initialization failed: " << Pa_GetErrorText(err) << endl;
        return 1;
    }

    data.phase = 0.0;
    data.phaseIncrement = 2.0 * M_PI * 440.0 / SAMPLE_RATE;

    // GLFW initialization
    if (!glfwInit())
    {
        cout << "Failed to initialize GLFW" << endl;
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE); // Hide the window until we are done with the setup

    GLFWwindow* window = glfwCreateWindow(800, 600, "ImGui GLFW Example", NULL, NULL);
    if (!window)
    {
        cout << "Failed to open GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    glfwShowWindow(window);

    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Create a simple button
        if (ImGui::Button("Play Sound"))
        {
            err = Pa_OpenDefaultStream(&stream, 0, /* no input channels */
                                       2, /* stereo output */
                                       paFloat32, /* 32 bit floating point output */
                                       SAMPLE_RATE,
                                       FRAMES_PER_BUFFER, /* frames per buffer */
                                       paCallback,
                                       &data);
            if (err != paNoError)
            {
                cout << "PortAudio error: " << Pa_GetErrorText(err) << endl;
            }

            err = Pa_StartStream(stream);
            if (err != paNoError)
            {
                cout << "PortAudio error: " << Pa_GetErrorText(err) << endl;
            }
        }
                // Yeni bir buton ekleyerek HTTP isteği yap
        if (ImGui::Button("Fetch Username")) {
            PerformHttpRequest(); // HTTP isteğini yap
        }
        
        // HTTP isteği sonucunu göster
        ImGui::Text("HTTP Response: %s", httpResult.c_str());

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // PortAudio cleanup
    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();

    // GLFW cleanup
    glfwTerminate();

    return 0;
}
