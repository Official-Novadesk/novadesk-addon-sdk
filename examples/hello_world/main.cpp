#include <NovadeskAPI/novadesk_addon.h>
#include <thread>
#include <atomic>

std::atomic<bool> g_Running = false;
novadesk::JsFunction* g_Callback = nullptr;
novadesk::Dispatcher* g_Dispatcher = nullptr;

// This function runs on the MAIN THREAD
void OnPulse(void* data) {
    if (g_Callback && g_Callback->IsValid()) {
        g_Callback->Call("Pulse from Main Thread!");
    }
}

// The initialization function is called when the addon is loaded.
NOVADESK_ADDON_INIT(ctx, hMsgWnd) {
    // Use the C++ helper to manage registration
    novadesk::Addon addon(ctx);
    g_Running = true;
    g_Dispatcher = new novadesk::Dispatcher(hMsgWnd);

    // Register properties and functions easily
    addon.RegisterStringFunction("hello", "Hello from the abstracted Addon SDK!");
    addon.RegisterString("version", "1.0.0");

    // Nest objects for cleaner APIs
    addon.RegisterObject("utils", [](novadesk::Addon& utils) {
        utils.RegisterNumber("id", 123);
        utils.RegisterStringFunction("ping", "pong");
        
        // Return arrays of data
        utils.RegisterArray("tags", {"cpp", "native", "addon"});
        utils.RegisterArray("versions", {1.0, 1.1, 2.0});
    });

    // JavaScript Callbacks (Event Hooks)
    addon.RegisterFunction("onEvent", [](duk_context* ctx) -> duk_ret_t {
        novadesk::JsFunction cb(ctx, 0); // Capture the function at index 0
        if (cb.IsValid()) {
            cb.Call("Hello from C++ Callback!");
        }
        return 0;
    });

    // Persistent Background Thread
    addon.RegisterFunction("startPulse", [](duk_context* ctx) -> duk_ret_t {
        if (g_Callback) delete g_Callback;
        g_Callback = new novadesk::JsFunction(ctx, 0);

        std::thread([]() {
            while (g_Running) {
                std::this_thread::sleep_for(std::chrono::seconds(2));
                if (g_Running && g_Dispatcher) {
                    // Safely dispatch to main thread
                    g_Dispatcher->Dispatch(OnPulse);
                }
            }
        }).detach();

        return 0;
    });
}

// Optional: The unload function is called when the script reloads or Novadesk exits.
NOVADESK_ADDON_UNLOAD() {
    g_Running = false;
    if (g_Callback) { delete g_Callback; g_Callback = nullptr; }
    if (g_Dispatcher) { delete g_Dispatcher; g_Dispatcher = nullptr; }
    // Perform any necessary cleanup here (no Duktape context available here)
}
