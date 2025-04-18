void aging_replace(int op_type, int vpn) {
    int pos = find_page(vpn);
    if (pos != -1) {
        page_table[pos].R = true;
        if (op_type == 1) page_table[pos].M = true;
    } else {
        // ищем пустую ячейку
        for (int i = 0; i < PAGE_FRAME_COUNT; ++i) {
            if (page_table[i].vpn == -1) {
                page_table[i].vpn = vpn;
                page_table[i].R = true;
                page_table[i].M = (op_type == 1);
                page_table[i].counter = 0;
                print_table();
                return;
            }
        }

        // Если нет свободного места - СДВИГАЕМ счетчики
        for (auto& entry : page_table) {
            entry.counter >>= 1;
            if (entry.R) {
                entry.counter |= 0x80; // установить старший бит
            }
        }

        // Выбираем страницу с минимальным счётчиком
        int min_value = 256;
        vector<int> candidates;
        for (int i = 0; i < PAGE_FRAME_COUNT; ++i) {
            if (page_table[i].counter < min_value) {
                candidates.clear();
                candidates.push_back(i);
                min_value = page_table[i].counter;
            } else if (page_table[i].counter == min_value) {
                candidates.push_back(i);
            }
        }

        int victim = candidates[uniform_rnd(0, candidates.size() - 1)];

        page_table[victim].vpn = vpn;
        page_table[victim].R = true;
        page_table[victim].M = (op_type == 1);
        page_table[victim].counter = 0;
    }
    print_table();
}
