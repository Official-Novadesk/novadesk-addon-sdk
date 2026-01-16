/* Copyright (C) 2026 Novadesk Project 
 *
 * This Source Code Form is subject to the terms of the GNU General Public
 * License; either version 2 of the License, or (at your option) any later
 * version. If a copy of the GPL was not distributed with this file, You can
 * obtain one at <https://www.gnu.org/licenses/gpl-2.0.html>. */

#pragma once

#include <Windows.h>
#include "duktape/duktape.h"

#ifdef __cplusplus
extern "C" {
#endif

// Define the initialization function signature
typedef void (*NovadeskAddonInitFn)(duk_context* ctx, HWND hMsgWnd);

// Define the cleanup function signature (optional)
typedef void (*NovadeskAddonUnloadFn)();

// Entry Point Macros
#define NOVADESK_ADDON_INIT(ctx, hMsgWnd) extern "C" __declspec(dllexport) void NovadeskAddonInit(duk_context* ctx, HWND hMsgWnd)
#define NOVADESK_ADDON_UNLOAD() extern "C" __declspec(dllexport) void NovadeskAddonUnload()

#ifdef __cplusplus
}

#include <vector>
#include <string>

// C++ Helper Utilities
namespace novadesk {
    class JsFunction {
    public:
        JsFunction(duk_context* ctx, duk_idx_t idx) : m_ctx(ctx) {
            if (duk_is_function(m_ctx, idx)) {
                m_ptr = duk_get_heapptr(m_ctx, idx);
            } else {
                m_ptr = nullptr;
            }
        }

        bool IsValid() const { return m_ptr != nullptr; }

        // Call the JS function with 0 arguments
        void Call() {
            if (!IsValid()) return;
            duk_push_heapptr(m_ctx, m_ptr);
            if (duk_pcall(m_ctx, 0) != 0) {
                duk_pop(m_ctx); // pop error
            } else {
                duk_pop(m_ctx); // pop result
            }
        }

        // Call the JS function with a string argument
        void Call(const char* arg) {
            if (!IsValid()) return;
            duk_push_heapptr(m_ctx, m_ptr);
            duk_push_string(m_ctx, arg);
            if (duk_pcall(m_ctx, 1) != 0) {
                duk_pop(m_ctx);
            } else {
                duk_pop(m_ctx);
            }
        }

        // Call the JS function with a number argument
        void Call(double arg) {
            if (!IsValid()) return;
            duk_push_heapptr(m_ctx, m_ptr);
            duk_push_number(m_ctx, arg);
            if (duk_pcall(m_ctx, 1) != 0) {
                duk_pop(m_ctx);
            } else {
                duk_pop(m_ctx);
            }
        }

    private:
        duk_context* m_ctx;
        void* m_ptr;
    };

    class Dispatcher {
    public:
        Dispatcher(HWND hMsgWnd) : m_hWnd(hMsgWnd) {}

        // Dispatch a task to the main thread.
        // The callback should be a static function: void MyCallback(void* data)
        void Dispatch(void (*fn)(void*), void* data = nullptr) {
            if (m_hWnd) {
                PostMessage(m_hWnd, WM_NOVADESK_DISPATCH, (WPARAM)fn, (LPARAM)data);
            }
        }

    private:
        HWND m_hWnd;
        static const UINT WM_NOVADESK_DISPATCH = WM_USER + 101;
    };

    class Addon {
    public:
        Addon(duk_context* ctx) : m_ctx(ctx) {
            duk_push_object(m_ctx);
        }

        // Register a nested object
        template<typename F>
        void RegisterObject(const char* name, F populateFunc) {
            duk_push_object(m_ctx);
            Addon sub(m_ctx, true);
            populateFunc(sub);
            duk_put_prop_string(m_ctx, -2, name);
        }

        // Register a C-style function
        void RegisterFunction(const char* name, duk_c_function func, duk_idx_t nargs = 0) {
            duk_push_c_function(m_ctx, func, nargs);
            duk_put_prop_string(m_ctx, -2, name);
        }

        // Register a function that returns a constant string
        void RegisterStringFunction(const char* name, const char* value) {
            duk_push_c_function(m_ctx, [](duk_context* ctx) -> duk_ret_t {
                duk_push_current_function(ctx);
                duk_get_prop_string(ctx, -1, "\xff" "data");
                return 1;
            }, 0);
            duk_push_string(m_ctx, value);
            duk_put_prop_string(m_ctx, -2, "\xff" "data");
            duk_put_prop_string(m_ctx, -2, name);
        }

        // Register a string property
        void RegisterString(const char* name, const char* value) {
            if (value) {
                duk_push_string(m_ctx, value);
            } else {
                duk_push_null(m_ctx);
            }
            duk_put_prop_string(m_ctx, -2, name);
        }

        // Register a number property
        void RegisterNumber(const char* name, double value) {
            duk_push_number(m_ctx, value);
            duk_put_prop_string(m_ctx, -2, name);
        }

        // Register a boolean property
        void RegisterBool(const char* name, bool value) {
            duk_push_boolean(m_ctx, value ? 1 : 0);
            duk_put_prop_string(m_ctx, -2, name);
        }

        // Register an array of strings
        void RegisterArray(const char* name, const std::vector<std::string>& values) {
            duk_idx_t arr_idx = duk_push_array(m_ctx);
            for (size_t i = 0; i < values.size(); ++i) {
                duk_push_string(m_ctx, values[i].c_str());
                duk_put_prop_index(m_ctx, arr_idx, (duk_uarridx_t)i);
            }
            duk_put_prop_string(m_ctx, -2, name);
        }

        // Register an array of numbers
        void RegisterArray(const char* name, const std::vector<double>& values) {
            duk_idx_t arr_idx = duk_push_array(m_ctx);
            for (size_t i = 0; i < values.size(); ++i) {
                duk_push_number(m_ctx, values[i]);
                duk_put_prop_index(m_ctx, arr_idx, (duk_uarridx_t)i);
            }
            duk_put_prop_string(m_ctx, -2, name);
        }

    private:
        // Internal constructor for sub-objects
        Addon(duk_context* ctx, bool) : m_ctx(ctx) {}

        duk_context* m_ctx;
    };
}
#endif
