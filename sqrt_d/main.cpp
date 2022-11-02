#include <bits/stdc++.h>

#define isz(x) (int)x.size()

using namespace std;


const int block_size = 256;
const int UNDEF = -1;

auto &setmax(auto &x) { return x; }

auto &setmin(auto &x) { return x; }

auto &setmax(auto &x, const auto &y, const auto &... pack) {
    return setmax(x = (x < y) ? y : x, pack...);
}

auto &setmin(auto &x, const auto &y, const auto &... pack) {
    return setmin(x = (y < x) ? y : x, pack...);
}

class R_decomposition{
public:
    explicit R_decomposition(int size){
        cnt_blocks = size / block_size + 1;
    }
    int get_block(int &pos) const {
        return pos / block_size;
    }
    int begin_block(int &block) const{
        return block * block_size;
    }
    int end_block(int &block) const{
        return block * block_size + block_size;
    }
    void init_sum(vector <int> &a){
        sum_in_block.assign(cnt_blocks,0);
        for (int i = 0;i < isz(a);i++){
            sum_in_block[i / block_size] += a[i];
        }
    }
    void init_min(vector <int> &a){
        min_in_block.assign(cnt_blocks,1e9);
        for (int i = 0;i < isz(a);i++){
            setmin(min_in_block[i / block_size],a[i]);
        }
    }
    void init_new_val(vector <int> &a){
        val_in_block.assign(cnt_blocks,UNDEF);
        val_in_pos.assign(isz(a),UNDEF);
        for (int i = 0;i < isz(a);i++){
            val_in_pos[i] = a[i];
        }
    }
    void update_sum(vector <int> &a,int pos,int new_val){
        pos--;
        sum_in_block[pos / block_size] -= a[pos];
        a[pos] = new_val;
        sum_in_block[pos / block_size] += new_val;
    }
    void update_min(vector <int> &a,int pos,int new_val){
        pos--;
        a[pos] = new_val;
        setmin(min_in_block[get_block(pos)],a[pos]);
    }
    void update_val(vector <int> &a,int l,int r,int new_val){
        l--; r--;
        int left_block = get_block(l),right_block = get_block(r);
        if (left_block != right_block) {
            if (val_in_block[left_block] > UNDEF) {
                for (int i = begin_block(left_block); i < l; i++) val_in_pos[i] = val_in_block[left_block];
            }
            val_in_block[left_block] = UNDEF;
            for (int i = l; i < end_block(left_block); i++) val_in_pos[i] = new_val;
            for (int i = left_block + 1; i < right_block; i++) val_in_block[i] = new_val;
            for (int i = begin_block(right_block);i <= min(isz(a) - 1,r);i++) val_in_pos[i] = new_val;
            if (val_in_block[right_block] > UNDEF){
                for (int i = r + 1;i <= min(end_block(right_block) - 1,isz(a) - 1);i++) val_in_pos[i] = val_in_block[right_block];
            }
            val_in_block[right_block] = UNDEF;
        }
        else{
            if (val_in_block[left_block] > UNDEF){
                for (int i = begin_block(left_block);i < l;i++) val_in_pos[i] = val_in_block[left_block];
                for (int i = r + 1;i <= min(end_block(right_block) - 1,isz(a) - 1);i++)  val_in_pos[i] = val_in_block[right_block];
            }
            for (int i = l;i <= r;i++) val_in_pos[i] = new_val;
            val_in_block[left_block] = UNDEF;
        }
        /*
        for (int i = 0;i < isz(a);i++){
            if (val_in_block[i / block_size] == -1) std::cout << val_in_pos[i] << ' ';
            else std::cout << val_in_block[i / block_size] << ' ';
        }
         */
    }
    long long get_sum(vector <int> &a,int l,int r) const{
        l--; r--;
        long long sum = 0;
        int left_block = get_block(l),right_block = get_block(r);
        if (left_block != right_block){
            for (int i = l;i < end_block(left_block);i++) sum += a[i];
            for (int i = left_block + 1;i < right_block;i++) sum += sum_in_block[i];
            for (int i = begin_block(right_block);i <= min(isz(a) - 1,r);i++) sum += a[i];
        }
        else{
            for (int i = l;i <= r;i++) sum += a[i];
        }
        return sum;
    }
    int get_min(vector <int> &a,int l,int r) const{
        l--; r--;
        int min_val = 1e9;
        int left_block = get_block(l),right_block = get_block(r);
        if (left_block != right_block){
            for (int i = l;i < end_block(left_block);i++){
                setmin(min_val,a[i]);
            }
            for (int i = left_block + 1;i < right_block;i++){
                setmin(min_val,min_in_block[i]);
            }
            for (int i = begin_block(right_block);i <= min(isz(a) - 1,r);i++){
                setmin(min_val,a[i]);
            }
        }
        else{
            for (int i = l;i <= r;i++) setmin(min_val,a[i]);
        }
        return min_val;
    }
private:
    int cnt_blocks;
    vector <long long> sum_in_block;
    vector <int> min_in_block;
    vector <int> val_in_pos,val_in_block;
};

int main() {
    int n; std::cin >> n;
    vector <int> a(n); // {-1,4,95,3,1,7,951,5,-10,123}
    for (auto& val : a) std::cin >> val;
    R_decomposition sqrt_d_sum(n),sqrt_d_min(n),sqrt_d_new_val(n);
    sqrt_d_sum.init_sum(a); sqrt_d_min.init_min(a); sqrt_d_new_val.init_new_val(a);
    std::cout << sqrt_d_sum.get_sum(a,2,9) << "\n"; // sum = 1056 OK
    sqrt_d_sum.update_sum(a,7,15); // old a[6] = 951,new a[6] = 15
    std::cout << sqrt_d_sum.get_sum(a,2,9) << "\n"; // sum = 120 OK
    std::cout << sqrt_d_min.get_min(a,4,7) << "\n"; // min = 1 OK
    sqrt_d_min.update_min(a,5,23); // old a[4] = 1,new a[4] = 23
    std::cout << sqrt_d_min.get_min(a,4,7)  << "\n"; // min = 3 OK
    sqrt_d_new_val.update_val(a,5,9,23); // {-1,4,95,3,23,23,23,23,23,123} OK
    sqrt_d_new_val.update_val(a,7,10,1024); // {-1,4,95,3,23,23,1024,1024,1024,1024} OK
}
