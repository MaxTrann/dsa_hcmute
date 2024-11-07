#include <bits/stdc++.h>
using namespace std;

#define LH -1 // cây con trái cao hơn phải 1 đơn vị
#define EH 0  // cây con trái bằng cây con phải
#define RH 1  // cây con phải cao hơn cây trái 1 đơn vị

struct AVLNode {
    string word; // từ cần lưu trong từ điển
    string meaning; // nghĩa của từ
    char balFactor; // chỉ số cân bằng
    AVLNode *pLeft, *pRight;
};

// Cấu trúc dữ liệu của cây
typedef AVLNode *AVLTree;

// Hàm khởi tạo cây
void initTree(AVLTree &t) {
    t = NULL;
}

// Hàm tạo một node
AVLNode *createNode(string word, string meaning) {
    AVLNode *p = new AVLNode;
    p->word = word;
    p->meaning = meaning;
    p->balFactor = EH; // Khởi tạo chỉ số cân bằng bằng 0 (cân bằng)
    p->pLeft = NULL;
    p->pRight = NULL;
    return p;
}

// Xoay phải
void LL(AVLTree &t) {
    AVLNode *t1 = t->pLeft;
    t->pLeft = t1->pRight;
    t1->pRight = t;
    t->balFactor = t1->balFactor = EH;
    t = t1;
}

// Xoay trái-phải
void LR(AVLTree &t) {
    AVLNode *t1 = t->pLeft;
    AVLNode *t2 = t1->pRight;
    t->pLeft = t2->pRight;
    t2->pRight = t;
    t1->pRight = t2->pLeft;
    t2->pLeft = t1;

    switch (t2->balFactor) {
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

// Xoay trái
void RR(AVLTree &t) {
    AVLNode *t1 = t->pRight;
    t->pRight = t1->pLeft;
    t1->pLeft = t;
    t->balFactor = t1->balFactor = EH;
    t = t1;
}

// Xoay phải-trái
void RL(AVLTree &t) {
    AVLNode *t1 = t->pRight;
    AVLNode *t2 = t1->pLeft;
    t->pRight = t2->pLeft;
    t2->pLeft = t;
    t1->pLeft = t2->pRight;
    t2->pRight = t1;

    switch (t2->balFactor) {
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

// Thêm từ mới vào từ điển
int insertNode(AVLTree &t, string word, string meaning) {
    int res;
    if (t == NULL) {
        t = createNode(word, meaning);
        return 1; // chiều cao của cây tăng
    }
    if (word < t->word) {
        res = insertNode(t->pLeft, word, meaning);
        if (!res) return 0;
        switch (t->balFactor) {
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
    } else if (word > t->word) {
        res = insertNode(t->pRight, word, meaning);
        if (!res) return 0;
        switch (t->balFactor) {
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
    } else {
        cout << "Tu nay da ton tai trong tu dien.\n";
        return 0; // Từ đã tồn tại
    }
    return res;
}
void deleteNode(AVLTree &t, string x, bool &shorter) {
    if (t == NULL) return;

    if (x < t->word) {
        deleteNode(t->pLeft, x, shorter);
        if (shorter) { // Cần kiểm tra và cân bằng nếu cây con trái ngắn đi
            switch (t->balFactor) {
                case LH:
                    t->balFactor = EH;
                    shorter = true;
                    break;
                case EH:
                    t->balFactor = RH;
                    shorter = false;
                    break;
                case RH:
                    if (t->pRight->balFactor == RH) RR(t);
                    else RL(t);
                    shorter = true;
                    break;
            }
        }
    } else if (x > t->word) {
        deleteNode(t->pRight, x, shorter);
        if (shorter) { // Cần kiểm tra và cân bằng nếu cây con phải ngắn đi
            switch (t->balFactor) {
                case RH:
                    t->balFactor = EH;
                    shorter = true;
                    break;
                case EH:
                    t->balFactor = LH;
                    shorter = false;
                    break;
                case LH:
                    if (t->pLeft->balFactor == LH) LL(t);
                    else LR(t);
                    shorter = true;
                    break;
            }
        }
    } else { // x == t->data, tìm thấy node cần xóa
        AVLNode *p = t;
        if (t->pLeft == NULL) {
            t = t->pRight;
            shorter = true;
        } else if (t->pRight == NULL) {
            t = t->pLeft;
            shorter = true;
        } else {
            AVLNode *s = t, *q = t->pLeft;
            while (q->pRight != NULL) { // tìm phần tử lớn nhất bên trái
                s = q;
                q = q->pRight;
            }
            t->word = q->word; // gán giá trị của q vào t
            deleteNode(t->pLeft, q->word, shorter); // xóa q
            if (shorter) { // cân bằng lại nếu cây con trái ngắn đi
                switch (t->balFactor) {
                    case LH:
                        t->balFactor = EH;
                        shorter = true;
                        break;
                    case EH:
                        t->balFactor = RH;
                        shorter = false;
                        break;
                    case RH:
                        if (t->pRight->balFactor == RH) RR(t);
                        else RL(t);
                        shorter = true;
                        break;
                }
            }
        }
        delete p;
    }
}


// Tìm kiếm từ trong từ điển
bool searchWord(AVLTree t, string word) {
    if (t == NULL) return false;
    if (word < t->word)
        return searchWord(t->pLeft, word);
    else if (word > t->word)
        return searchWord(t->pRight, word);
    else {
        cout << "Tu: " << t->word << ", Nghia: " << t->meaning << endl;
        return true;
    }
}

// Duyệt theo thứ tự tăng dần
void printInOrder(AVLTree t) {
    if (t != NULL) {
        printInOrder(t->pLeft);
        cout << "Tu: " << t->word << ", Nghia: " << t->meaning << endl;
        printInOrder(t->pRight);
    }
}

int main() {
    AVLTree t;
    initTree(t);
    int choice;
    do {
        cout << "\nTu dien AVL\n";
        cout << "1. Them tu moi\n";
        cout << "2. Tim tu\n";
        cout << "3. Hien thi tu dien\n";
        cout << "4. Xoa tu\n";
        cout << "0. Thoat\n";
        cout << "Nhap lua chon cua ban: ";
        cin >> choice;
        cin.ignore();
        if (choice == 1) {
            string word, meaning;
            cout << "Nhap tu: ";
            getline(cin, word);
            cout << "Nhap nghia: ";
            getline(cin, meaning);
            insertNode(t, word, meaning);
        } else if (choice == 2) {
            string word;
            cout << "Nhap tu can tim: ";
            getline(cin, word);
            if (!searchWord(t, word)) {
                cout << "Khong tim thay tu trong tu dien.\n";
            }
        } else if (choice == 3) {
            cout << "Cac tu trong tu dien:\n";
            printInOrder(t);
        }
        else if (choice == 4){
            bool shorter = false;
            cout << "Nhap tu can xoa: ";string word; cin >> word;
            deleteNode(t, word, shorter);
        }
    } while (choice != 0);
    return 0;
}
