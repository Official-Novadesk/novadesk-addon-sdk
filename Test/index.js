import { addon } from "novadesk";

console.log("=========================================");
console.log("   Novadesk Multi-Addon Test Suite       ");
console.log("=========================================");

try {
    const loaded = [];

    console.log("\n[1/3] Loading Hello World...");
    const hello = addon.load("../dist/x64/Debug/hello_world/hello_world.dll");
    if (hello) {
        loaded.push(hello);
        console.log("  Success! Version: " + hello.version);
        console.log("  Addon says: " + hello.hello());

        hello.onEvent(function (msg) {
            console.log("  [Hello Event]: " + msg);
        });
    } else {
        console.error("  Failed to load hello_world.dll");
    }

    console.log("\n[2/3] Loading Math Utilities...");
    const math = addon.load("../dist/x64/Debug/math_utils/math_utils.dll");
    if (math) {
        loaded.push(math);
        console.log("  Success!");
        console.log("  Test: 15 + 25 = " + math.sum(15, 25));
        console.log("  Test: 100 - 42 = " + math.subtract(100, 42));
        console.log("  Test: 12 * 12 = " + math.multiply(12, 12));

        try {
            console.log("  Testing Error: " + math.sum(10, "not a number"));
        } catch (e) {
            console.log("  Caught expected error: " + e.message);
        }
    } else {
        console.error("  Failed to load math_utils.dll");
    }

    console.log("\n[3/3] Loading CPU Monitor...");
    const cpu = addon.load("../dist/x64/Debug/cpu_monitor/cpu_monitor.dll");
    if (cpu) {
        loaded.push(cpu);
        console.log("  Success! Initializing real-time tracker...");
        cpu.start(function (usage) {
            console.log("  [CPU]: " + usage.toFixed(2) + "%");
        });
    } else {
        console.error("  Failed to load cpu_monitor.dll");
    }

    console.log("\nAll tests initialized. System ready.");
    console.log("Will auto-unload addons in 10 seconds...");
    console.log("=========================================");

    setTimeout(function () {
        console.log("\n[Cleanup] Unloading all addons...");

        loaded.forEach(function (a, idx) {
            try {
                const ok = a.unload();
                console.log("  Successfully unloaded addon #" + (idx + 1) + ": " + ok);
            } catch (e) {
                console.error("  Failed to unload addon #" + (idx + 1) + ": " + e.message);
            }
        });

        console.log("\n[Cleanup] Finished. You can close Novadesk now.");
    }, 10000);

} catch (e) {
    console.error("Failed to initialize Novadesk Addon SDK: " + e.message);
}
