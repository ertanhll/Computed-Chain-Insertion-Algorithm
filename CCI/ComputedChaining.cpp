#include "ComputedChaining.h" //Halil Ertan 20220702109
using namespace std;

ComputedChaining::ComputedChaining(int table_size) {
    for (int i = 0; i < table_size; ++i) {
        data_vec.push_back(entry());
    }
}
int ComputedChaining::hash(int key) {
    return key % data_vec.size();
}
int ComputedChaining::hash2(int key) {
    int step = (key / data_vec.size()) % data_vec.size();
    if (step == 0)
        step = 1;
    return step;
}
void ComputedChaining::insert(int key) {
    int index = hash(key);

    if (data_vec[index].data != -1) {
        if (isThisYourHome(key)) {
            int increment;
            while (data_vec[index].link != -1) {
                increment = hash2(data_vec[index].data);
                index = (index + increment * data_vec[index].link) % data_vec.size();
            }

            int current_index = index;
            increment = hash2(data_vec[current_index].data);

            int i = current_index;
            int num_link_count = 0;
            while (true) {
                if (data_vec[i].data == -1) {
                    data_vec[i].data = key;
                    data_vec[current_index].link = num_link_count;
                    break;
                }
                i = (i + increment) % data_vec.size();
                num_link_count++;
            }
        }
        else {
            vector<int> chain_list = find_chain(data_vec[index].data);
            vector<int> reinserted_list;

            for (int current_index: chain_list) {
                reinserted_list.push_back(data_vec[current_index].data);
                data_vec[current_index].data = -1;
                data_vec[current_index].link = -1;
            }

            data_vec[index].data = key;

            for (int value: reinserted_list) {
                insert(value);
            }
        }

    }else {
        data_vec[index].data = key;
        data_vec[index].link = -1;
    }
}

int ComputedChaining::find_index(int key) {
    int idx = hash(key);
    int inc = hash2(key);

    if (data_vec[idx].data == key)
        return idx;

    else {
        while (data_vec[idx].data != -1) {
            if (data_vec[idx].data != key) {
                inc = hash2(data_vec[idx].data);
                idx = (idx + data_vec[idx].link * inc) % data_vec.size();
            }

            else
                return idx;
        }
    }

    return -1;
}

bool ComputedChaining::isThisYourHome(int key) {
    return hash(first_of_my_chain(key)) == hash(key);
}

int ComputedChaining::first_of_my_chain(int key) {
    return data_vec[hash(key)].data;
}

int ComputedChaining::find_num_probes(int key){

    int num_probe = 1;
    int increment;
    int index= hash(key);

    if (first_of_my_chain(key) == key)
        return num_probe;

    while (data_vec[index].data != -1) {
        int current_key = data_vec[index].data;
        if (current_key == key) {
            return num_probe;
        }
        increment = hash2(current_key);
        index = (index + data_vec[index].link * increment) % data_vec.size();
        num_probe++;
    }

    return -1;
}

vector<int> ComputedChaining::find_chain(int key) {
    vector<int> chain;
    int index = hash(key);
    int inc;

    while (data_vec[index].data != -1) {
        chain.push_back(index);

        if (data_vec[index].link != -1) {
            inc = hash2(data_vec[index].data);
            index = (index + data_vec[index].link * inc) % data_vec.size();
        }
        else
            break;
    }

    return chain;
}
