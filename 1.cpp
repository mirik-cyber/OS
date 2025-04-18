#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

const int PAGE_FRAME_COUNT = 14;
const int TIMER_INTERVAL = 5;

struct PageEntry {
    int vpn;         // Virtual Page Number
    bool R;          // Reference bit
    bool M;          // Modification bit
    uint8_t counter; // For Aging algorithm

    PageEntry() : vpn(-1), R(false), M(false), counter(0) {}
};

vector<PageEntry> page_table(PAGE_FRAME_COUNT);
int access_count = 0;

int find_page(int vpn) {
    for (int i = 0; i < PAGE_FRAME_COUNT; ++i)
        if (page_table[i].vpn == vpn)
            return i;
    return -1;
}

void print_table() {
    for (int i = 0; i < PAGE_FRAME_COUNT; ++i) {
        if (page_table[i].vpn == -1)
            cout << "#";
        else
            cout << page_table[i].vpn;
        if (i != PAGE_FRAME_COUNT - 1)
            cout << " ";
    }
    cout << "\n";
}

void timer_interrupt() {
    for (auto& entry : page_table)
        entry.R = false;
}

int uniform_rnd(int a, int b) {
    return rand() % (b - a + 1) + a;
}

// ---------------- FIFO ----------------
void fifo_replace(int op_type, int vpn) {
    static queue<int> fifo_queue;

    int pos = find_page(vpn);
    if (pos != -1) {
        page_table[pos].R = true;
        if (op_type == 1) page_table[pos].M = true;
    } else {
        for (int i = 0; i < PAGE_FRAME_COUNT; ++i) {
            if (page_table[i].vpn == -1) {
                page_table[i].vpn = vpn;
                page_table[i].R = true;
                page_table[i].M = (op_type == 1);
                fifo_queue.push(i);
                print_table();
                return;
            }
        }
        int victim = fifo_queue.front(); fifo_queue.pop();
        page_table[victim].vpn = vpn;
        page_table[victim].R = true;
        page_table[victim].M = (op_type == 1);
        fifo_queue.push(victim);
    }
    print_table();
}

// ---------------- AGING ----------------
void aging_replace(int op_type, int vpn) {
    int pos = find_page(vpn);
    if (pos != -1) {
        page_table[pos].R = true;
        if (op_type == 1) page_table[pos].M = true;
    } else {
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

        for (auto& entry : page_table) {
            entry.counter >>= 1;
            if (entry.R) entry.counter |= 0x80;
        }

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

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: ./lab4 <algorithm_number>\n";
        return 1;
    }

    srand(time(0));
    int algorithm = stoi(argv[1]);
    int op_type, vpn;

    while (cin >> op_type >> vpn) {
        ++access_count;

        if (algorithm == 1)
            fifo_replace(op_type, vpn);
        else if (algorithm == 2)
            aging_replace(op_type, vpn);
        else {
            cerr << "Invalid algorithm number: " << algorithm << "\n";
            return 2;
        }

        if (access_count % TIMER_INTERVAL == 0)
            timer_interrupt();
    }

    return 0;
}
