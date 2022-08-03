nvrreturn void panic(rstr fmt, ...) {
	va_list args;
	va_start(args, fmt);
	str s = tvsprintf(fmt, args);
	va_end(args);
	tfprintf(stderr, "panic: %s\n\n", s);
	*((volatile u32*)0) = 42;
	exit(EXIT_FAILURE);
}


nvrreturn void todo() {
	panic("TODO");
}