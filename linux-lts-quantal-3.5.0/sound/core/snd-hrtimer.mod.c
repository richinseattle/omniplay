#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

MODULE_INFO(intree, "Y");

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xee584c90, "module_layout" },
	{ 0x440a4045, "kmalloc_caches" },
	{ 0x22e85e41, "hrtimer_forward" },
	{ 0xb75d662a, "hrtimer_cancel" },
	{ 0xb9fa1af3, "snd_timer_global_new" },
	{ 0xb5cb8145, "hrtimer_get_res" },
	{ 0x50eedeb8, "printk" },
	{ 0xb4390f9a, "mcount" },
	{ 0x8750ccc5, "hrtimer_start" },
	{ 0x7807eace, "kmem_cache_alloc_trace" },
	{ 0x37a0cba, "kfree" },
	{ 0x8f69ad65, "snd_timer_interrupt" },
	{ 0xea905975, "hrtimer_init" },
	{ 0xb0a30bb8, "snd_timer_global_register" },
	{ 0x402f042b, "snd_timer_global_free" },
	{ 0xe914e41e, "strcpy" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=snd-timer";


MODULE_INFO(srcversion, "CB38AF4A2452507777D37FA");
