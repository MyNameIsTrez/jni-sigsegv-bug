char define_type[] = "h";

long long globals_size = 8;

void *on_fns[] = {0};

char *strings[] = {};

long long resources_size = 8;

long long entities_size = 8;

void game_fn_define_h(int x);

void define(void) {
    game_fn_define_h(42);
}

void init_globals(long long *g, long long id) {
    *g = id;
}
