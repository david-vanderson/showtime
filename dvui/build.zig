const std = @import("std");

pub fn build(b: *std.Build) !void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const dvui_dep = b.dependency("dvui", .{ .target = target, .optimize = optimize, .sdl3 = true });

    const exe = b.addExecutable(.{
        .name = "dvui",
        .root_source_file = b.path("dvui.zig"),
        .target = target,
        .optimize = optimize,
    });

    exe.root_module.addImport("dvui", dvui_dep.module("dvui_sdl"));
    const install_step = b.addInstallArtifact(exe, .{});
    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(&install_step.step);

    const run_step = b.step("run", "Run");
    run_step.dependOn(&run_cmd.step);

    b.getInstallStep().dependOn(run_step);
}
