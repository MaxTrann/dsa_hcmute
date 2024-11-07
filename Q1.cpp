#include <bits/stdc++.h>
using namespace std;
#define ElementType int

#define LH -1 // cây con trái cao hơn phải 1 đơn vị
#define EH 0  // cây con trái bằng cây con phải
#define RH 1  // cây con phải cao hơn cây trái 1 đơn vị 

struct AVLNode{
    char balFactor; // chỉ số cân bằng
    ElementType data;
    AVLNode *pLeft, *pRight;
};

// Cấu trúc dữ liệu của cây
typedef AVLNode *AVLTree;

// Hàm khởi tạo cây
void initTree(AVLTree &t){
    t = NULL;
}

// Hàm tạo 1 node:
AVLNode *createNode(ElementType x){
    AVLNode *p = new AVLNode;
    p->data = x;
    p->balFactor = EH; // Khởi tạo chỉ số cân bằng bằng 0 (cân bằng)
    p->pLeft = NULL;
    p->pRight = NULL;
    return p;
}

// Kiểm tra cây rỗng
bool isEmpty(AVLTree &t){
    return t == NULL;
}

// Trường hợp 1: Cân bằng trái-trái -> xoay phải
void LL(AVLTree &t){
    AVLNode *t1 = t->pLeft;
    t->pLeft = t1->pRight;
    t1->pRight = t;
    t->balFactor = t1->balFactor = EH;
    t = t1;
}

// Trường hợp 2: Cân bằng trái-phải -> xoay trái, sau đó xoay phải
void LR(AVLTree &t){
    AVLNode *t1 = t->pLeft;
    AVLNode *t2 = t1->pRight;
    t->pLeft = t2->pRight;
    t2->pRight = t;
    t1->pRight = t2->pLeft;
    t2->pLeft = t1;

    switch(t2->balFactor){
        case LH:
            t->balFactor = RH;
            t1->balFactor = EH;
            break;
        case EH:
            t->balFactor = t1->balFactor = EH;
            break;
        case RH:
            t->balFactor = EH;
            t1->balFactor = LH;
            break;
    }
    t2->balFactor = EH;
    t = t2;
}

// Trường hợp 3: Cân bằng phải-phải -> xoay trái
void RR(AVLTree &t){
    AVLNode *t1 = t->pRight;
    t->pRight = t1->pLeft;
    t1->pLeft = t;
    t->balFactor = t1->balFactor = EH;
    t = t1;
}

// Trường hợp 4: Cân bằng phải-trái -> xoay phải, sau đó xoay trái
void RL(AVLTree &t){
    AVLNode *t1 = t->pRight;
    AVLNode *t2 = t1->pLeft;
    t->pRight = t2->pLeft;
    t2->pLeft = t;
    t1->pLeft = t2->pRight;
    t2->pRight = t1;

    switch(t2->balFactor){
        case LH:
            t->balFactor = EH;
            t1->balFactor = RH;
            break;
        case EH:
            t->balFactor = t1->balFactor = EH;
            break;
        case RH:
            t->balFactor = LH;
            t1->balFactor = EH;
            break;
    }
    t2->balFactor = EH;
    t = t2;
}

// Thêm 1 phần tử vào cây AVL
int insertNode(AVLTree &t, ElementType x){
    int res;
    if (t == NULL){
        t = createNode(x);
        return 1; // chiều cao của cây tăng
    }
    if (x < t->data){
        res = insertNode(t->pLeft, x);
        if (!res) return 0;
        switch(t->balFactor){
            case LH:
                if (t->pLeft->balFactor == LH) LL(t);
                else LR(t);
                res = 0;
                break;
            case EH:
                t->balFactor = LH;
                res = 1;
                break;
            case RH:
                t->balFactor = EH;
                res = 0;
                break;
        }
    }
    else if (x > t->data){
        res = insertNode(t->pRight, x);
        if (!res) return 0;
        switch(t->balFactor){
            case LH:
                t->balFactor = EH;
                res = 0;
                break;
            case EH:
                t->balFactor = RH;
                res = 1;
                break;
            case RH:
                if (t->pRight->balFactor == RH) RR(t);
                else RL(t);
                res = 0;
                break;
        }
    }
    else return 0; // x đã tồn tại trong cây
    return res;
}

// Xóa 1 phần tử trong cây
void deleteNode(AVLTree &t, ElementType x){
    if (t == NULL) return;
    if (x < t->data){
        deleteNode(t->pLeft, x);
        switch(t->balFactor){
            case LH:
                t->balFactor = EH;
                break;
            case EH:
                t->balFactor = RH;
                break;
            case RH:
                if (t->pRight->balFactor == RH) RR(t);
                else RL(t);
                break;
        }
    }
    else if (x > t->data){
        deleteNode(t->pRight, x);
        switch(t->balFactor){
            case LH:
                if (t->pLeft->balFactor == LH) LL(t);
                else LR(t);
                break;
            case EH:
                t->balFactor = LH;
                break;
            case RH:
                t->balFactor = EH;
                break;
        }
    }
    else{ // x == t->data
        AVLNode *p = t;
        if (t->pLeft == NULL)
            t = t->pRight;
        else if (t->pRight == NULL)
            t = t->pLeft;
        else{
            AVLNode *s = t, *q = t->pLeft;
            while (q->pRight != NULL){ // tìm phần tử lớn nhất bên trái
                s = q;
                q = q->pRight;
            }
            t->data = q->data; // gán giá trị của q vào t
            p = q;
            if (s != t) // nếu q ko phải con trực tiếp của t
                s->pRight = q->pLeft; // cập nhật phải cho s
            else 
                s->pLeft = q->pLeft; // cập nhật trái cho s 
            delete p;
        }
    }
}
// BT1: Hàm tính chiều cao của cây
int heightTree(AVLTree &t){
    if (t == NULL) return 0;
    int h1 = heightTree(t->pLeft);
    int h2 = heightTree(t->pRight);
    return max(h1, h2) + 1;
}
//2a. Hàm đếm số node nhánh của cây
int countLeafNode (AVLTree t){
    if (t == NULL) return 0;
    if (t->pLeft == NULL && t->pRight == NULL) // điều kiện là 1 node lá
        return 1;
    return countLeafNode(t->pLeft) + countLeafNode(t->pRight); // đệ quy
}
//2b. Hàm đếm số lá trên cây
int countBranchNode(AVLTree t){
    if (t == NULL) return 0;
    if (t->pLeft != NULL || t->pRight != NULL){
        return 1 + countBranchNode(t->pLeft) + countBranchNode(t->pRight);
    }
    return 0;
}
//2c. Hàm đếm bao nhiêu phần tử chẵn trên cây
int countEvenNode(AVLTree t){
    if (t == NULL) return 0;
    if (t->data % 2 == 0){
        return 1 + countEvenNode(t->pRight) + countEvenNode(t->pLeft);
    }
    return countEvenNode(t->pLeft) + countEvenNode(t->pRight);
}

//2d. Tính tổng giá trị các nút trên cây nhị phân gồm các giá trị nguyên
int sumNode(AVLTree t){
    if (t == NULL) return 0;
    return t->data + sumNode(t->pLeft) + sumNode(t->pRight);
}
//2e. Tìm giá trị nguyên lớn nhất và nhỏ nhất trong số các phần nguyên trên cây nhị phân
void findMaxMin(AVLTree t, int &maxx, int &minn){
    if (t == NULL) return;
    if (t->data > maxx) maxx = t->data;
    if (t->data < minn) minn = t->data;
    findMaxMin(t->pLeft, maxx, minn);
    findMaxMin(t->pRight, maxx, minn);
}
//2f. Đếm có bao nhiêu giá trị phân biệt trong dãy số cho trước
void countDistinctNode(AVLTree t, set<int> &se){
    if (t == NULL) return;
    se.insert(t->data);
    countDistinctNode(t->pLeft, se);
    countDistinctNode(t->pRight, se);
}
//2g. Với mỗi giá trị phân biệt, cho biết số lượng phần tử
void countDinstinctNodeValue(AVLTree t, map<int, int> &mp){
    if (t == NULL) return;
    mp[t->data]++;
    countDinstinctNodeValue(t->pLeft, mp);
    countDinstinctNodeValue(t->pRight, mp);
}
// xuất cây theo kiểu tiền tự
void printPreOrder(AVLTree t){
    if (t != NULL){
        cout << t->data << " ";
        printPreOrder(t->pLeft);
        printPreOrder(t->pRight);
    }
}
// xuất cây theo kiểu trung tự
void printInOrder(AVLTree t){
    if (t != NULL){
        printInOrder(t->pLeft);
        cout << t->data << " ";
        printInOrder(t->pRight);
    }
}
// xuất cây theo kiểu hậu tự 
void printPostOrder(AVLTree t){
    if (t != NULL){
        printPostOrder(t->pLeft);
        printPostOrder(t->pRight);
        cout << t->data << " ";
    }
}

int main()
{
AVLTree t;
    initTree(t);
    while(1){
        cout << "Nhap phan tu them vao cay: ";
        int x; cin >> x;
        if (x == 0){
            break;
        }
        else{
            insertNode(t, x);
        }
    }
    cout << "Duyet cay theo kieu tien tu: ";
    printPreOrder(t);
    cout << endl;
    cout << "Duyet cay theo kieu trung tu: ";
    printInOrder(t);
    cout << endl;
    cout << "Duyet cay theo kieu hau tu: ";
    printPostOrder(t);
    cout << endl;
    cout << "Chieu cao cua cay: " << heightTree(t) << endl;
    cout << "So node la cua cay: " << countLeafNode(t) << endl;
    cout << "So node nhanh cua cay: " << countBranchNode(t) << endl;
    cout << "So node chan cua cay: " << countEvenNode(t) << endl;
    cout << "Tong cac node cua cay: " << sumNode(t) << endl;
    int max = t->data, min = t->data;
    findMaxMin(t,max,min);
    cout << "Max: " << max << endl;
    cout << "Min: " << min << endl;
    set<int> se;
    countDistinctNode(t, se);
    cout << "So node phan biet cua cay: " << se.size() << endl;
    map<int,int> mp;
    countDinstinctNodeValue(t, mp);
    cout << "Moi phan tu rieng biet va so luong: " << endl;
    for (auto it : mp){
        cout << it.first << " " << it.second << endl;
    }
    return 0;
}
