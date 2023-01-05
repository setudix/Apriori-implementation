#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>

using namespace std;

const int N = 100;

using Transaction = struct{
    string t_id;
    set<int> items;
};

// using Frequency = struct{
//     set<int> itemset;
//     int cnt;
// };

vector<Transaction> transactions;
map<set<int>, int> frequency;
vector<vector<set<int>>> l,c;

int cur_itemset_no = 0;
int min_support;
int last_c_size = -1;
bool found_final_candidate = 0;

void get_input(){
    cin >> min_support;   
    string t_id;
    int element;
    while (cin >> t_id){
        Transaction temp;
        temp.t_id = t_id;
        int item_no;
        cin >> item_no;
        for (int i = 0; i < item_no; i++){
            int x;
            cin >> x;
            temp.items.insert(x);
        }
        transactions.push_back(temp);
        temp.items.clear();
        temp.t_id.clear();
    }
}
void generate_first_itemset(){
    l.push_back({});
    set<set<int>> all_sets;
    for (auto [tid, s] : transactions){
        for (int element : s){
            all_sets.insert({element});
        }
    }
    
    for (auto s : all_sets){
        l[cur_itemset_no].push_back(s);
    }
    // cout << "l" << cur_itemset_no + 1 << " generated\n";    
    cur_itemset_no++;
}
void generate_itemset(){
    if (cur_itemset_no == 0){
        generate_first_itemset();
        return;
    }
    int idx = cur_itemset_no - 1;
    auto itemsets_vec = c[idx];
    set<set<int>> all_l;
    for (auto current_set : itemsets_vec){
        for (auto next_set : itemsets_vec){
            set<int> temp;
            temp.insert(current_set.begin(), current_set.end());
            temp.insert(next_set.begin(), next_set.end());

                // for (int i : temp){
                //     cout << i << " ";
                // }
                // cout << endl;
            if ((int)temp.size() == cur_itemset_no + 1){
                // for (int i : temp){
                //     cout << i << " ";
                // }
                // cout << endl;
                all_l.insert(temp);
            }
            temp.clear();
        }
    }
    l.push_back({});
    for (auto s : all_l){
        l.back().push_back(s);
    }
    // cout << "l" << cur_itemset_no + 1 << " generated\n";    
    cur_itemset_no++;
}

int count_frequency(set<int> &cur){ 
    int cnt = 0;
    for (auto [tid, s] : transactions){
        set<int> temp;
        temp.insert(s.begin(), s.end());
        temp.insert(cur.begin(), cur.end());
        if (temp.size() == s.size()){
            cnt++;
        }
        temp.clear();
    } 
    return cnt;
}

void generate_candidates(){
    int idx = cur_itemset_no - 1;
    auto itemset_vec = l.back();
    set<set<int>> all_c;
    for (auto itemset : itemset_vec){
        int cnt = count_frequency(itemset);
        frequency[itemset] = cnt;
        if (cnt >= min_support){
            all_c.insert(itemset);
        }
    }
    if ((int)all_c.size () != 0){
        c.push_back({});
        for (auto s : all_c){
            c.back().push_back(s);
        }
        // cout << "c" << cur_itemset_no << " generated\n";    
    }
    else{
        found_final_candidate = 1;
        cout << "Candidiates found!\n";
    }
}

void print(vector<Transaction> &t){
    cout << "Transactions: \n";
    for (auto [tid, s] : t){
        cout << tid << " -- ";
        for (int i : s){
            cout << i << " ";
        }
        cout << endl;
    }
}

void print(vector<set<int>> &itemset_vec){
    cout << "Final candidates: \n";
    for (auto s : itemset_vec){
        for (int element : s){
            cout << element << " ";
        }
        cout << endl;
    }
}

int main(){
    get_input();
    while (!found_final_candidate){
        generate_itemset();
        generate_candidates();
    }
    // cout << "set no " << cur_itemset_no << endl;
    print(transactions);
    print(c.back());
}