const std = @import("std");
const builtin = @import("builtin");
const dvui = @import("dvui");

var name: [50:0]u8 = ("Alice" ++ [_]u8{0} ** 45).*;
var editing: bool = false;

fn myFrame() !void {
    {
        var hbox = try dvui.box(@src(), .horizontal, .{ .min_size_content = .{ .h = 38 } });
        defer hbox.deinit();

        if (!editing) {
            try dvui.label(@src(), "Hello:    {s}", .{std.mem.sliceTo(&name, 0)}, .{ .gravity_y = 0.5 });
        } else {
            try dvui.label(@src(), "Hello:", .{}, .{ .gravity_y = 0.5 });
            var te = try dvui.textEntry(@src(), .{ .text = .{ .buffer = &name } }, .{ .gravity_y = 0.5, .min_size_content = .{ .w = 80 } });
            if (te.enter_pressed) {
                editing = false;
            }
            te.deinit();
        }
    }

    if (try dvui.button(@src(), if (editing) "Done" else "Edit", .{}, .{})) {
        editing = !editing;
    }

    try dvui.label(@src(), "{d:0>3.0} fps", .{dvui.FPS()}, .{});
}

pub fn main() !void {
    var gpa_instance = std.heap.GeneralPurposeAllocator(.{}){};
    const gpa = gpa_instance.allocator();

    // OS window
    var backend = try dvui.backend.initWindow(.{
        .allocator = gpa,
        .size = .{ .w = 400.0, .h = 300.0 },
        .vsync = true,
        .title = "dvui Zig",
    });
    backend.initial_scale = 2; // make demo bigger
    defer backend.deinit();

    // dvui Window maps to OS window
    var win = try dvui.Window.init(@src(), gpa, backend.backend(), .{});
    defer win.deinit();

    win.theme = win.themes.get("Adwaita Dark").?;

    main_loop: while (true) {
        // frame setup
        const nstime = win.beginWait(backend.hasEvent());
        try win.begin(nstime);

        // queue all events
        const quit = try backend.addAllEvents(&win);
        if (quit) break :main_loop;

        // clear background
        _ = dvui.backend.c.SDL_SetRenderDrawColor(backend.renderer, 0, 0, 0, 255);
        _ = dvui.backend.c.SDL_RenderClear(backend.renderer);

        try myFrame();

        // frame end
        const end_micros = try win.end(.{});

        backend.setCursor(win.cursorRequested());
        backend.textInputRect(win.textInputRequested());
        backend.renderPresent();

        // wait for next event or animation
        const wait_event_micros = win.waitTime(end_micros, null);
        backend.waitEventTimeout(wait_event_micros);
    }
}
