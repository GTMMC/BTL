#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <climits>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iterator>
#include <limits>
#include <sstream>
#include <string>
using namespace std;
typedef struct sanpham { 
    int maSanPham;
    string tenSanPham;
    string danhMuc;
    int gia;
    string hinhAnh;
    string moTa;
    int hienTrangChu;
    int hienAoKhoac;
    string kichCo;
    int soLuong;
    int soLuongDaBan;
} Sp;
typedef struct Nut { 
    Sp data;
    Nut *dauKichCo;
    Nut *kichCoTiepTheo;
    Nut *sanPhamTiepTheo;
} Nut;
class KhoHang{
    public:
        KhoHang();
        ~KhoHang();
        Nut *cuoi;
        Nut *dau;
        Nut *dauKichCo;
        Nut *TaoSanPham(int maSanPham, string tenSanPham);
        Nut *TaoKichCo(string kichCo);
        void XoaDuLieu();
        void ThemLoaiSanPham(int maSanPham, string tenSanPham, string danhMuc,
            int gia, const string& hinhAnh, const string& moTa);
        bool ThemKichCo(int soLuong, string kichCo, int maSanPham);
        bool NhapThemSoLuong(int maSanPham, string kichCo, int soLuong);
        void InSanPham(Nut *nut);
        void NhapSanPham(int soLuong, string kichCo, int maSanPham);
        bool KiemTraTrungMa(int maSanPham);
        bool CapNhatSanPham(int maSanPham, const string& tenSanPham, const string& danhMuc);
        bool CapNhatHienThi(int maSanPham, int hienTrangChu, int hienAoKhoac);
        bool CapNhatAnh(int maSanPham, const string& hinhAnh);
        bool XoaSanPham(int maSanPham);
        bool TimSanPham(int maSanPham, string kichCo, int& soLuongTon, int& soLuongDaBan);
        bool LuuJSON(const string& tenTep);
        bool BanSanPham(int maSanPham, string kichCo, int soLuongBan);
};
KhoHang::KhoHang() : cuoi(NULL), dau(NULL), dauKichCo(NULL) {}
KhoHang::~KhoHang() {
    XoaDuLieu();
}
bool KhoHang::NhapThemSoLuong(int maSanPham, string kichCo, int soLuong){
    if (soLuong <= 0) return false;
    Nut *nut = dau;
    while(nut != NULL){
        if(nut->data.maSanPham == maSanPham){
            Nut *nutCon = nut->dauKichCo;
            while(nutCon != NULL){
                if(nutCon->data.kichCo == kichCo){
                    if (nutCon->data.soLuong > numeric_limits<int>::max() - soLuong) return false;
                    nutCon->data.soLuong += soLuong;
                    return true;
                }
                nutCon = nutCon->kichCoTiepTheo;
            }
            return ThemKichCo(soLuong, kichCo, maSanPham);
        }
        nut = nut->sanPhamTiepTheo;
    }
    return false;
}
bool KhoHang::BanSanPham(int maSanPham, string kichCo, int soLuongBan){
    if (soLuongBan <= 0) return false;
    Nut *nut = dau;
    while(nut != NULL){
        if(nut->data.maSanPham == maSanPham){
            Nut *nutCon = nut->dauKichCo;
            while(nutCon != NULL){
                if(nutCon->data.kichCo == kichCo){
                    if (nutCon->data.soLuong < soLuongBan ||
                        nutCon->data.soLuongDaBan > numeric_limits<int>::max() - soLuongBan) return false;
                    nutCon->data.soLuong -= soLuongBan;
                    nutCon->data.soLuongDaBan += soLuongBan;
                    return true;
                }
                nutCon = nutCon->kichCoTiepTheo;
            }
            return false;
        }
        nut = nut->sanPhamTiepTheo;
    }
    return false;
}
bool NhapSoNguyen(const string& loiNhac, int giaTriToiThieu, int& giaTri) {
    while (true) {
        cout << loiNhac;
        if (cin >> giaTri) {
            if (giaTri >= giaTriToiThieu) return true;
            cout << "Gia tri phai lon hon hoac bang " << giaTriToiThieu << ".\n";
        } else {
            if (cin.eof()) return false;
            cout << "Vui long nhap mot so nguyen hop le.\n";
            cin.clear();
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string MaHoaJSON(const string& vanBan) {
    string chuoiDaMaHoa;
    for (size_t chiSo = 0; chiSo < vanBan.size(); ++chiSo) {
        unsigned char kyTu = static_cast<unsigned char>(vanBan[chiSo]);
        switch (kyTu) {
            case '"': chuoiDaMaHoa += "\\\""; break;
            case '\\': chuoiDaMaHoa += "\\\\"; break;
            case '\b': chuoiDaMaHoa += "\\b"; break;
            case '\f': chuoiDaMaHoa += "\\f"; break;
            case '\n': chuoiDaMaHoa += "\\n"; break;
            case '\r': chuoiDaMaHoa += "\\r"; break;
            case '\t': chuoiDaMaHoa += "\\t"; break;
            default:
                if (kyTu < 0x20) {
                    const char* chuoiHex = "0123456789abcdef";
                    chuoiDaMaHoa += "\\u00";
                    chuoiDaMaHoa += chuoiHex[kyTu >> 4];
                    chuoiDaMaHoa += chuoiHex[kyTu & 0x0F];
                } else {
                    chuoiDaMaHoa += static_cast<char>(kyTu);
                }
        }
    }
    return chuoiDaMaHoa;
}

bool KhoHang::LuuJSON(const string& tenTep) {
    ofstream tep(tenTep.c_str());
    if (!tep) {
        cerr << "Khong the mo file de ghi: " << tenTep << endl;
        return false;
    }

    tep << "{\n  \"san_pham\": [\n";
    Nut *nut = dau;
    while (nut != NULL) {
        tep << "    {\"id\": " << nut->data.maSanPham
             << ", \"ten\": \"" << MaHoaJSON(nut->data.tenSanPham)
             << "\", \"danhMuc\": \"" << MaHoaJSON(nut->data.danhMuc)
             << "\", \"gia\": " << nut->data.gia
             << ", \"img\": \"" << MaHoaJSON(nut->data.hinhAnh)
             << "\", \"desc\": \"" << MaHoaJSON(nut->data.moTa)
             << "\", \"hienTrangChu\": " << nut->data.hienTrangChu
             << ", \"hienAoKhoac\": " << nut->data.hienAoKhoac << ", \"sizes\": [";
        Nut *nutCon = nut->dauKichCo;
        while (nutCon != NULL) {
            tep << "{\"size\": \"" << MaHoaJSON(nutCon->data.kichCo)
                 << "\", \"so_luong\": " << nutCon->data.soLuong
                 << ", \"da_ban\": " << nutCon->data.soLuongDaBan << "}";
            nutCon = nutCon->kichCoTiepTheo;
            if (nutCon != NULL) tep << ", ";
        }
        tep << "]}" << (nut->sanPhamTiepTheo != NULL ? "," : "") << "\n";
        nut = nut->sanPhamTiepTheo;
    }
    tep << "  ]\n}\n";

    if (!tep) {
        cerr << "Loi khi ghi file: " << tenTep << endl;
        return false;
    }
    return true;
}

bool KhoHang::TimSanPham(int maSanPham, string kichCo, int& soLuongTon, int& soLuongDaBan){
    Nut *nut=dau;
     while (nut!=NULL){
         if (nut->data.maSanPham==maSanPham){
             Nut *nutCon=nut->dauKichCo;
             while (nutCon!=NULL){
                 if (nutCon->data.kichCo==kichCo){
                     soLuongTon = nutCon->data.soLuong;
                     soLuongDaBan = nutCon->data.soLuongDaBan;
                     return true;
                 }
                 nutCon=nutCon->kichCoTiepTheo;
             }
             return false;
         }
         nut=nut->sanPhamTiepTheo;
     }
     return false;
}
void KhoHang::XoaDuLieu(){
    Nut *sanPham = dau;
    while (sanPham != NULL) {
        Nut *sanPhamTiepTheo = sanPham->sanPhamTiepTheo;
        Nut *kichCo = sanPham->dauKichCo;
        while (kichCo != NULL) {
            Nut *kichCoTiepTheo = kichCo->kichCoTiepTheo;
            delete kichCo;
            kichCo = kichCoTiepTheo;
        }
        delete sanPham;
        sanPham = sanPhamTiepTheo;
    }
    dau = cuoi = dauKichCo = NULL;
}
Nut* KhoHang::TaoSanPham(int maSanPham, string tenSanPham){
    Nut *nut = new Nut();
    if(nut == NULL){
        cout << "Cap phat bo nho loi!";
        return NULL;
    }
    else{
        nut->data.maSanPham = maSanPham;
        nut->data.tenSanPham = tenSanPham;
        nut->data.danhMuc = "Chua phan loai";
        nut->data.gia = 0;
        nut->data.hinhAnh = "";
        nut->data.moTa = "";
        nut->data.hienTrangChu = 1;
        nut->data.hienAoKhoac = 1;
        nut->data.kichCo = "";
        nut->data.soLuong = 0;
        nut->data.soLuongDaBan = 0;
        nut->sanPhamTiepTheo = NULL;
        nut->kichCoTiepTheo = NULL;
        nut->dauKichCo = NULL;
        return nut;
    }
}
Nut* KhoHang::TaoKichCo(string kichCo){
    Nut *nut = new Nut();
    if(nut == NULL){
        cout << "Cap phat bo nho loi!";
        return NULL;
    }
    else{
        nut->data.maSanPham = 0;
        nut->data.tenSanPham = "";
        nut->data.danhMuc = "";
        nut->data.gia = 0;
        nut->data.hinhAnh = "";
        nut->data.moTa = "";
        nut->data.hienTrangChu = 1;
        nut->data.hienAoKhoac = 1;
        nut->data.kichCo = kichCo;
        nut->data.soLuong = 0;
        nut->data.soLuongDaBan = 0;
        nut->kichCoTiepTheo = NULL;
        nut->dauKichCo = NULL;
        nut->sanPhamTiepTheo = NULL;
        return nut;
    }
}
void KhoHang::ThemLoaiSanPham(int maSanPham, string tenSanPham, string danhMuc,
    int gia, const string& hinhAnh, const string& moTa){
    Nut *nutMoi = TaoSanPham(maSanPham, tenSanPham);
    if (nutMoi == NULL) return;
    nutMoi->data.danhMuc = danhMuc;
    nutMoi->data.gia = gia;
    nutMoi->data.hinhAnh = hinhAnh;
    nutMoi->data.moTa = moTa;
    if(dau == NULL){
        dau = cuoi = nutMoi;
    }
    else{
        cuoi->sanPhamTiepTheo = nutMoi;
        cuoi = nutMoi;
    }
}
bool KhoHang::ThemKichCo(int soLuong, string kichCo, int maSanPham){
    if (soLuong < 0 || kichCo.empty()) return false;
    Nut *nut = dau;
    while(nut != NULL){
        if(nut->data.maSanPham == maSanPham){
            Nut *existing = nut->dauKichCo;
            while (existing != NULL) {
                if (existing->data.kichCo == kichCo) return false;
                existing = existing->kichCoTiepTheo;
            }
            Nut *nutMoi = TaoKichCo(kichCo);
            if (nutMoi == NULL) return false;
            nutMoi->data.soLuong = soLuong;
            if(nut->dauKichCo == NULL){
                nut->dauKichCo = nutMoi;
            }
            else{
                Nut *nutCon = nut->dauKichCo;
                while(nutCon->kichCoTiepTheo != NULL){
                    nutCon = nutCon->kichCoTiepTheo;
                }
                nutCon->kichCoTiepTheo = nutMoi;
            }
            return true;
        }
        nut = nut->sanPhamTiepTheo;
    }
    return false;
}
void KhoHang::InSanPham(Nut *nut){
    if (nut == NULL) return;
    cout << "ID: " << nut->data.maSanPham << ", Ten: " << nut->data.tenSanPham << endl;
    Nut *nutCon = nut->dauKichCo;
    while(nutCon != NULL){
        cout << "Size: " << nutCon->data.kichCo << ", So luong: " << nutCon->data.soLuong << endl;
        nutCon = nutCon->kichCoTiepTheo;
    }
}
bool KhoHang::KiemTraTrungMa(int maSanPham) {
    Nut *nut = dau;
    while (nut != NULL) {
        if (nut->data.maSanPham == maSanPham) return true;
        nut = nut->sanPhamTiepTheo;
    }
    return false;
}
bool KhoHang::CapNhatSanPham(int maSanPham, const string& tenSanPham, const string& danhMuc) {
    Nut *nut = dau;
    while (nut != NULL) {
        if (nut->data.maSanPham == maSanPham) {
            nut->data.tenSanPham = tenSanPham;
            nut->data.danhMuc = danhMuc;
            return true;
        }
        nut = nut->sanPhamTiepTheo;
    }
    return false;
}
bool KhoHang::CapNhatHienThi(int maSanPham, int hienTrangChu, int hienAoKhoac) {
    if ((hienTrangChu != 0 && hienTrangChu != 1) ||
        (hienAoKhoac != 0 && hienAoKhoac != 1)) return false;
    Nut *nut = dau;
    while (nut != NULL) {
        if (nut->data.maSanPham == maSanPham) {
            nut->data.hienTrangChu = hienTrangChu;
            nut->data.hienAoKhoac = hienAoKhoac;
            return true;
        }
        nut = nut->sanPhamTiepTheo;
    }
    return false;
}
bool KhoHang::CapNhatAnh(int maSanPham, const string& hinhAnh) {
    Nut *nut = dau;
    while (nut != NULL) {
        if (nut->data.maSanPham == maSanPham) {
            nut->data.hinhAnh = hinhAnh;
            return true;
        }
        nut = nut->sanPhamTiepTheo;
    }
    return false;
}
bool KhoHang::XoaSanPham(int maSanPham) {
    Nut *truoc = NULL;
    Nut *nut = dau;
    while (nut != NULL && nut->data.maSanPham != maSanPham) {
        truoc = nut;
        nut = nut->sanPhamTiepTheo;
    }
    if (nut == NULL) return false;
    if (truoc == NULL) dau = nut->sanPhamTiepTheo;
    else truoc->sanPhamTiepTheo = nut->sanPhamTiepTheo;
    if (cuoi == nut) cuoi = truoc;
    Nut *kichCo = nut->dauKichCo;
    while (kichCo != NULL) {
        Nut *tiepTheo = kichCo->kichCoTiepTheo;
        delete kichCo;
        kichCo = tiepTheo;
    }
    delete nut;
    return true;
}

struct GiaTriJSON;

struct NodeJSONEntry {
    string key;
    GiaTriJSON* value;
    NodeJSONEntry* next;
    NodeJSONEntry(const string& khoa, GiaTriJSON* giaTri)
        : key(khoa), value(giaTri), next(NULL) {}
};

struct NodeJSONList {
    GiaTriJSON* value;
    NodeJSONList* next;
    explicit NodeJSONList(GiaTriJSON* giaTri) : value(giaTri), next(NULL) {}
};

struct FormField {
    string key;
    string value;
    FormField* next;
    FormField(const string& khoa, const string& giaTri)
        : key(khoa), value(giaTri), next(NULL) {}
};

struct GiaTriJSON {
    enum Kieu { DOI_TUONG, MANG, CHUOI, SO, KHAC } kieu;
    NodeJSONEntry* doiTuong;
    NodeJSONList* mang;
    string chuoi;
    int so;

    GiaTriJSON() : kieu(KHAC), doiTuong(NULL), mang(NULL), so(0) {}
    GiaTriJSON(const GiaTriJSON& other);
    GiaTriJSON& operator=(const GiaTriJSON& other);
    ~GiaTriJSON();
};

void XoaJSONObject(NodeJSONEntry*& head) {
    while (head != NULL) {
        NodeJSONEntry* next = head->next;
        delete head->value;
        delete head;
        head = next;
    }
}

void XoaJSONArray(NodeJSONList*& head) {
    while (head != NULL) {
        NodeJSONList* next = head->next;
        delete head->value;
        delete head;
        head = next;
    }
}

void ThemThuocTinh(NodeJSONEntry*& head, const string& key, const GiaTriJSON& value) {
    for (NodeJSONEntry* p = head; p != NULL; p = p->next) {
        if (p->key == key) {
            *p->value = value;
            return;
        }
    }
    NodeJSONEntry* node = new NodeJSONEntry(key, new GiaTriJSON(value));
    if (head == NULL) {
        head = node;
        return;
    }
    NodeJSONEntry* last = head;
    while (last->next != NULL) last = last->next;
    last->next = node;
}

void ThemPhanTuMang(NodeJSONList*& head, const GiaTriJSON& value) {
    NodeJSONList* node = new NodeJSONList(new GiaTriJSON(value));
    if (head == NULL) {
        head = node;
        return;
    }
    NodeJSONList* last = head;
    while (last->next != NULL) last = last->next;
    last->next = node;
}

const GiaTriJSON* TimThuocTinh(const NodeJSONEntry* head, const string& key) {
    for (const NodeJSONEntry* p = head; p != NULL; p = p->next) {
        if (p->key == key) return p->value;
    }
    return NULL;
}

GiaTriJSON::GiaTriJSON(const GiaTriJSON& other)
    : kieu(other.kieu), doiTuong(NULL), mang(NULL), chuoi(other.chuoi), so(other.so) {
    for (const NodeJSONEntry* p = other.doiTuong; p != NULL; p = p->next)
        ThemThuocTinh(doiTuong, p->key, *p->value);
    for (const NodeJSONList* p = other.mang; p != NULL; p = p->next)
        ThemPhanTuMang(mang, *p->value);
}

GiaTriJSON& GiaTriJSON::operator=(const GiaTriJSON& other) {
    if (this != &other) {
        XoaJSONObject(doiTuong);
        XoaJSONArray(mang);
        kieu = other.kieu;
        chuoi = other.chuoi;
        so = other.so;
        for (const NodeJSONEntry* p = other.doiTuong; p != NULL; p = p->next)
            ThemThuocTinh(doiTuong, p->key, *p->value);
        for (const NodeJSONList* p = other.mang; p != NULL; p = p->next)
            ThemPhanTuMang(mang, *p->value);
    }
    return *this;
}

GiaTriJSON::~GiaTriJSON() {
    XoaJSONObject(doiTuong);
    XoaJSONArray(mang);
}

class FormFields {
public:
    FormFields() : dau(NULL) {}
    ~FormFields() {
        while (dau != NULL) {
            FormField* next = dau->next;
            delete dau;
            dau = next;
        }
    }
    void Gan(const string& key, const string& value) {
        for (FormField* p = dau; p != NULL; p = p->next) {
            if (p->key == key) {
                p->value = value;
                return;
            }
        }
        FormField* node = new FormField(key, value);
        if (dau == NULL) {
            dau = node;
            return;
        }
        FormField* last = dau;
        while (last->next != NULL) last = last->next;
        last->next = node;
    }
    const string* Tim(const string& key) const {
        for (const FormField* p = dau; p != NULL; p = p->next) {
            if (p->key == key) return &p->value;
        }
        return NULL;
    }
private:
    FormField* dau;
};

class BoDocJSON {
public:
    explicit BoDocJSON(const string& duLieu) : duLieu(duLieu), viTri(0) {}
    bool Doc(GiaTriJSON& giaTri) {
        if (!DocGiaTri(giaTri, 0)) return false;
        BoQuaTrang();
        return viTri == duLieu.size();
    }
private:
    const string& duLieu;
    size_t viTri;
    void BoQuaTrang() {
        while (viTri < duLieu.size() && (duLieu[viTri] == ' ' || duLieu[viTri] == '\n' ||
               duLieu[viTri] == '\r' || duLieu[viTri] == '\t')) ++viTri;
    }
    bool KyTu(char kyTu) {
        BoQuaTrang();
        if (viTri >= duLieu.size() || duLieu[viTri] != kyTu) return false;
        ++viTri;
        return true;
    }
    static int Hex(char kyTu) {
        if (kyTu >= '0' && kyTu <= '9') return kyTu - '0';
        if (kyTu >= 'a' && kyTu <= 'f') return kyTu - 'a' + 10;
        if (kyTu >= 'A' && kyTu <= 'F') return kyTu - 'A' + 10;
        return -1;
    }
    static void ThemUTF8(string& ketQua, unsigned int ma) {
        if (ma <= 0x7F) ketQua += static_cast<char>(ma);
        else if (ma <= 0x7FF) {
            ketQua += static_cast<char>(0xC0 | (ma >> 6));
            ketQua += static_cast<char>(0x80 | (ma & 0x3F));
        } else if (ma <= 0xFFFF) {
            ketQua += static_cast<char>(0xE0 | (ma >> 12));
            ketQua += static_cast<char>(0x80 | ((ma >> 6) & 0x3F));
            ketQua += static_cast<char>(0x80 | (ma & 0x3F));
        } else {
            ketQua += static_cast<char>(0xF0 | (ma >> 18));
            ketQua += static_cast<char>(0x80 | ((ma >> 12) & 0x3F));
            ketQua += static_cast<char>(0x80 | ((ma >> 6) & 0x3F));
            ketQua += static_cast<char>(0x80 | (ma & 0x3F));
        }
    }
    bool Chuoi(string& ketQua) {
        if (!KyTu('"')) return false;
        while (viTri < duLieu.size()) {
            char kyTu = duLieu[viTri++];
            if (kyTu == '"') return true;
            if (static_cast<unsigned char>(kyTu) < 0x20) return false;
            if (kyTu != '\\') {
                ketQua += kyTu;
                continue;
            }
            if (viTri >= duLieu.size()) return false;
            char escape = duLieu[viTri++];
            switch (escape) {
                case '"': ketQua += '"'; break;
                case '\\': ketQua += '\\'; break;
                case '/': ketQua += '/'; break;
                case 'b': ketQua += '\b'; break;
                case 'f': ketQua += '\f'; break;
                case 'n': ketQua += '\n'; break;
                case 'r': ketQua += '\r'; break;
                case 't': ketQua += '\t'; break;
                case 'u': {
                    if (viTri + 4 > duLieu.size()) return false;
                    unsigned int ma = 0;
                    for (int i = 0; i < 4; ++i) {
                        int digit = Hex(duLieu[viTri++]);
                        if (digit < 0) return false;
                        ma = ma * 16 + static_cast<unsigned int>(digit);
                    }
                    if (ma >= 0xD800 && ma <= 0xDFFF) return false;
                    ThemUTF8(ketQua, ma);
                    break;
                }
                default: return false;
            }
        }
        return false;
    }
    bool DocGiaTri(GiaTriJSON& giaTri, int doSau) {
        if (doSau > 128) return false;
        BoQuaTrang();
        if (viTri >= duLieu.size()) return false;
        if (duLieu[viTri] == '{') {
            ++viTri;
            giaTri.kieu = GiaTriJSON::DOI_TUONG;
            if (KyTu('}')) return true;
            do {
                string khoa;
                GiaTriJSON thanhVien;
                if (!Chuoi(khoa) || !KyTu(':') || !DocGiaTri(thanhVien, doSau + 1)) return false;
                ThemThuocTinh(giaTri.doiTuong, khoa, thanhVien);
                if (KyTu('}')) return true;
            } while (KyTu(','));
            return false;
        }
        if (duLieu[viTri] == '[') {
            ++viTri;
            giaTri.kieu = GiaTriJSON::MANG;
            if (KyTu(']')) return true;
            do {
                GiaTriJSON muc;
                if (!DocGiaTri(muc, doSau + 1)) return false;
                ThemPhanTuMang(giaTri.mang, muc);
                if (KyTu(']')) return true;
            } while (KyTu(','));
            return false;
        }
        if (duLieu[viTri] == '"') {
            giaTri.kieu = GiaTriJSON::CHUOI;
            return Chuoi(giaTri.chuoi);
        }
        if (duLieu[viTri] == '-' || (duLieu[viTri] >= '0' && duLieu[viTri] <= '9')) {
            size_t batDau = viTri;
            if (duLieu[viTri] == '-') ++viTri;
            if (viTri >= duLieu.size() || duLieu[viTri] < '0' || duLieu[viTri] > '9') return false;
            while (viTri < duLieu.size() && duLieu[viTri] >= '0' && duLieu[viTri] <= '9') ++viTri;
            istringstream docSo(duLieu.substr(batDau, viTri - batDau));
            giaTri.kieu = GiaTriJSON::SO;
            return (docSo >> giaTri.so) && docSo.eof();
        }
        const char* hang[] = {"true", "false", "null"};
        for (int i = 0; i < 3; ++i) {
            string tu(hang[i]);
            if (duLieu.compare(viTri, tu.size(), tu) == 0) {
                viTri += tu.size();
                return true;
            }
        }
        return false;
    }
};

const GiaTriJSON* Lay(const GiaTriJSON& giaTri, const string& khoa) {
    return TimThuocTinh(giaTri.doiTuong, khoa);
}

bool NapKho(const string& tenTep, KhoHang& kho) {
    ifstream tep(tenTep.c_str(), ios::binary);
    if (!tep) {
        ifstream kiemTra(tenTep.c_str());
        if (kiemTra.good()) return false;
        return true;
    }
    string noiDung((istreambuf_iterator<char>(tep)), istreambuf_iterator<char>());
    GiaTriJSON goc;
    BoDocJSON boDoc(noiDung);
    if (!boDoc.Doc(goc) || goc.kieu != GiaTriJSON::DOI_TUONG) return false;
    const GiaTriJSON* sanPhamJSON = Lay(goc, "san_pham");
    if (sanPhamJSON == NULL || sanPhamJSON->kieu != GiaTriJSON::MANG) return false;
    kho.XoaDuLieu();
    for (const NodeJSONList* it = sanPhamJSON->mang; it != NULL; it = it->next) {
        const GiaTriJSON* id = Lay(*it->value, "id");
        const GiaTriJSON* ten = Lay(*it->value, "ten");
        const GiaTriJSON* sizes = Lay(*it->value, "sizes");
        const GiaTriJSON* danhMuc = Lay(*it->value, "danhMuc");
        const GiaTriJSON* gia = Lay(*it->value, "gia");
        const GiaTriJSON* hinhAnh = Lay(*it->value, "img");
        const GiaTriJSON* moTa = Lay(*it->value, "desc");
        const GiaTriJSON* hienTrangChu = Lay(*it->value, "hienTrangChu");
        const GiaTriJSON* hienAoKhoac = Lay(*it->value, "hienAoKhoac");
        if (it->value->kieu != GiaTriJSON::DOI_TUONG || id == NULL || id->kieu != GiaTriJSON::SO ||
            id->so < 1 || ten == NULL || ten->kieu != GiaTriJSON::CHUOI ||
            sizes == NULL || sizes->kieu != GiaTriJSON::MANG || kho.KiemTraTrungMa(id->so) ||
            (gia != NULL && (gia->kieu != GiaTriJSON::SO || gia->so < 0 || gia->so > INT_MAX)) ||
            (hienTrangChu != NULL && (hienTrangChu->kieu != GiaTriJSON::SO ||
                (hienTrangChu->so != 0 && hienTrangChu->so != 1))) ||
            (hienAoKhoac != NULL && (hienAoKhoac->kieu != GiaTriJSON::SO ||
                (hienAoKhoac->so != 0 && hienAoKhoac->so != 1))) ||
            (hinhAnh != NULL && hinhAnh->kieu != GiaTriJSON::CHUOI) ||
            (moTa != NULL && moTa->kieu != GiaTriJSON::CHUOI)) return false;
        kho.ThemLoaiSanPham(id->so, ten->chuoi,
            danhMuc != NULL && danhMuc->kieu == GiaTriJSON::CHUOI ? danhMuc->chuoi : "Chua phan loai",
            gia == NULL ? 0 : gia->so,
            hinhAnh == NULL ? "" : hinhAnh->chuoi,
            moTa == NULL ? "" : moTa->chuoi);
        kho.CapNhatHienThi(id->so,
            hienTrangChu == NULL ? 1 : hienTrangChu->so,
            hienAoKhoac == NULL ? 1 : hienAoKhoac->so);
        for (const NodeJSONList* size = sizes->mang; size != NULL; size = size->next) {
            const GiaTriJSON* tenSize = Lay(*size->value, "size");
            const GiaTriJSON* soLuong = Lay(*size->value, "so_luong");
            const GiaTriJSON* daBan = Lay(*size->value, "da_ban");
            if (size->value->kieu != GiaTriJSON::DOI_TUONG || tenSize == NULL || tenSize->kieu != GiaTriJSON::CHUOI ||
                soLuong == NULL || soLuong->kieu != GiaTriJSON::SO || soLuong->so < 0 ||
                (daBan != NULL && (daBan->kieu != GiaTriJSON::SO || daBan->so < 0)) ||
                !kho.ThemKichCo(soLuong->so, tenSize->chuoi, id->so)) return false;
            int tonKho = 0, soDaBan = 0;
            if (!kho.TimSanPham(id->so, tenSize->chuoi, tonKho, soDaBan)) return false;
            for (Nut* p = kho.dau; p != NULL; p = p->sanPhamTiepTheo) {
                if (p->data.maSanPham != id->so) continue;
                for (Nut* s = p->dauKichCo; s != NULL; s = s->kichCoTiepTheo) {
                    if (s->data.kichCo == tenSize->chuoi) s->data.soLuongDaBan = daBan == NULL ? 0 : daBan->so;
                }
            }
        }
    }
    return true;
}

string MaHoaJSON(const string& vanBan);
string JSONKho(const KhoHang& kho) {
    ostringstream out;
    out << "{\"san_pham\":[";
    bool dauTien = true;
    for (Nut* p = kho.dau; p != NULL; p = p->sanPhamTiepTheo) {
        if (!dauTien) out << ',';
        dauTien = false;
        out << "{\"id\":" << p->data.maSanPham << ",\"ten\":\"" << MaHoaJSON(p->data.tenSanPham)
            << "\",\"danhMuc\":\"" << MaHoaJSON(p->data.danhMuc) << "\",\"gia\":" << p->data.gia
            << ",\"img\":\"" << MaHoaJSON(p->data.hinhAnh) << "\",\"desc\":\""
            << MaHoaJSON(p->data.moTa) << "\",\"hienTrangChu\":" << p->data.hienTrangChu
            << ",\"hienAoKhoac\":" << p->data.hienAoKhoac << ",\"sizes\":[";
        bool sizeDauTien = true;
        for (Nut* s = p->dauKichCo; s != NULL; s = s->kichCoTiepTheo) {
            if (!sizeDauTien) out << ',';
            sizeDauTien = false;
            out << "{\"size\":\"" << MaHoaJSON(s->data.kichCo) << "\",\"so_luong\":" << s->data.soLuong
                << ",\"da_ban\":" << s->data.soLuongDaBan << '}';
        }
        out << "]}";
    }
    out << "]}";
    return out.str();
}

string GiaiMa(const string& vanBan) {
    string ketQua;
    for (size_t i = 0; i < vanBan.size(); ++i) {
        if (vanBan[i] == '+') ketQua += ' ';
        else if (vanBan[i] == '%' && i + 2 < vanBan.size()) {
            char hex[3] = {vanBan[i + 1], vanBan[i + 2], '\0'};
            char* end = NULL;
            long giaTri = strtol(hex, &end, 16);
            if (end != hex && *end == '\0') {
                ketQua += static_cast<char>(giaTri);
                i += 2;
            } else ketQua += vanBan[i];
        } else ketQua += vanBan[i];
    }
    return ketQua;
}

void DocForm(const string& noiDung, FormFields& truong) {
    size_t dau = 0;
    while (dau <= noiDung.size()) {
        size_t cuoi = noiDung.find('&', dau);
        if (cuoi == string::npos) cuoi = noiDung.size();
        size_t bang = noiDung.find('=', dau);
        if (bang != string::npos && bang < cuoi) {
            truong.Gan(GiaiMa(noiDung.substr(dau, bang - dau)),
                GiaiMa(noiDung.substr(bang + 1, cuoi - bang - 1)));
        }
        if (cuoi == noiDung.size()) break;
        dau = cuoi + 1;
    }
}

bool DocSo(const FormFields& truong, const string& khoa, int& so) {
    const string* giaTriText = truong.Tim(khoa);
    if (giaTriText == NULL || giaTriText->empty()) return false;
    istringstream in(*giaTriText);
    long giaTri;
    char thua;
    if (!(in >> giaTri) || (in >> thua) || giaTri < 1 || giaTri > INT_MAX) return false;
    so = static_cast<int>(giaTri);
    return true;
}

bool DocSoKhongAm(const FormFields& truong, const string& khoa, int& so) {
    const string* giaTriText = truong.Tim(khoa);
    if (giaTriText == NULL || giaTriText->empty()) return false;
    istringstream in(*giaTriText);
    long giaTri;
    char thua;
    if (!(in >> giaTri) || (in >> thua) || giaTri < 0 || giaTri > INT_MAX) return false;
    so = static_cast<int>(giaTri);
    return true;
}

void Gui(SOCKET client, int status, const string& lyDo, const string& noiDung) {
    ostringstream http;
    http << "HTTP/1.1 " << status << ' ' << lyDo << "\r\n"
         << "Content-Type: application/json; charset=utf-8\r\n"
         << "Content-Length: " << noiDung.size() << "\r\n"
         << "Access-Control-Allow-Origin: *\r\n"
         << "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n"
         << "Access-Control-Allow-Headers: Content-Type\r\n"
         << "Cache-Control: no-store\r\nConnection: close\r\n\r\n" << noiDung;
    string data = http.str();
    size_t sent = 0;
    while (sent < data.size()) {
        int n = send(client, data.data() + sent, static_cast<int>(data.size() - sent), 0);
        if (n <= 0) break;
        sent += static_cast<size_t>(n);
    }
}

string PhanHoi(const string& status, const string& message) {
    return "{\"status\":\"" + status + "\",\"message\":\"" + MaHoaJSON(message) + "\"}";
}

bool AnhHopLe(const string& hinhAnh) {
    if (hinhAnh.compare(0, 11, "data:image/") == 0) {
        return hinhAnh.find(";base64,") != string::npos && hinhAnh.size() <= 600 * 1024;
    }
    if (hinhAnh.size() > 2048 || hinhAnh.find_first_of(" \r\n\t") != string::npos) return false;
    return hinhAnh.compare(0, 7, "http://") == 0 ||
           hinhAnh.compare(0, 8, "https://") == 0;
}

bool Nhan(SOCKET client, string& request) {
    char buffer[4096];
    size_t headerEnd = string::npos, contentEnd = 0;
    while (request.size() < 1024 * 1024) {
        int n = recv(client, buffer, sizeof(buffer), 0);
        if (n <= 0) return false;
        request.append(buffer, static_cast<size_t>(n));
        if (headerEnd == string::npos) {
            headerEnd = request.find("\r\n\r\n");
            if (headerEnd != string::npos) {
                contentEnd = headerEnd + 4;
                istringstream headers(request.substr(0, headerEnd));
                string line;
                getline(headers, line);
                while (getline(headers, line)) {
                    if (!line.empty() && line[line.size() - 1] == '\r') line.erase(line.size() - 1);
                    size_t colon = line.find(':');
                    if (colon == string::npos) continue;
                    string key = line.substr(0, colon);
                    for (size_t i = 0; i < key.size(); ++i) {
                        if (key[i] >= 'A' && key[i] <= 'Z') key[i] += 'a' - 'A';
                    }
                    if (key == "content-length") {
                        istringstream len(line.substr(colon + 1));
                        size_t bodyLength;
                        if (!(len >> bodyLength) || bodyLength > 1024 * 1024) return false;
                        contentEnd += bodyLength;
                    }
                }
            }
        }
        if (headerEnd != string::npos && request.size() >= contentEnd) return true;
    }
    return false;
}

void XuLy(SOCKET client, KhoHang& kho, const string& tenTep) {
    string request;
    if (!Nhan(client, request)) {
        Gui(client, 400, "Bad Request", PhanHoi("error", "Yeu cau HTTP khong hop le."));
        return;
    }
    size_t headerEnd = request.find("\r\n\r\n");
    istringstream start(request.substr(0, request.find("\r\n")));
    string method, path, version;
    start >> method >> path >> version;
    if (method == "OPTIONS") {
        Gui(client, 204, "No Content", "");
        return;
    }
    size_t queryAt = path.find('?');
    string query = queryAt == string::npos ? "" : path.substr(queryAt + 1);
    if (queryAt != string::npos) path.erase(queryAt);
    if (method == "GET" && path == "/api/products") {
        Gui(client, 200, "OK", JSONKho(kho));
        return;
    }
    FormFields fields;
    DocForm(method == "GET" ? query :
        (headerEnd == string::npos ? "" : request.substr(headerEnd + 4)), fields);
    if (method == "GET" && path == "/api/stock") {
        int id, stock = 0, sold = 0;
        const string* size = fields.Tim("size");
        if (!DocSo(fields, "id", id) || size == NULL || size->empty()) {
            Gui(client, 400, "Bad Request", PhanHoi("error", "Can nhap ID san pham va size."));
            return;
        }
        if (!kho.TimSanPham(id, *size, stock, sold)) {
            Gui(client, 404, "Not Found", PhanHoi("error", "Khong tim thay san pham hoac size."));
            return;
        }
        ostringstream result;
        result << "{\"status\":\"success\",\"id\":" << id << ",\"size\":\"" << MaHoaJSON(*size)
               << "\",\"tonKho\":" << stock << ",\"daBan\":" << sold << '}';
        Gui(client, 200, "OK", result.str());
        return;
    }
    if (method != "POST") {
        Gui(client, 404, "Not Found", PhanHoi("error", "Khong tim thay API."));
        return;
    }
    if (path == "/api/add-product") {
        int id, gia = 0, soLuong = 0;
        const string* size = fields.Tim("size");
        const string* quantity = fields.Tim("sluong");
        const string* ten = fields.Tim("ten");
        const string* danhMuc = fields.Tim("danhMuc");
        const bool hasSize = size != NULL && !size->empty();
        const bool hasQuantity = quantity != NULL && !quantity->empty();
        if (!DocSo(fields, "id", id) || ten == NULL || ten->empty() ||
            danhMuc == NULL || danhMuc->empty()) {
            Gui(client, 400, "Bad Request", PhanHoi("error", "Can nhap ID hop le, ten va danh muc san pham."));
            return;
        }
        if (fields.Tim("gia") != NULL && !DocSoKhongAm(fields, "gia", gia)) {
            Gui(client, 400, "Bad Request", PhanHoi("error", "Gia san pham khong hop le."));
            return;
        }
        if (hasSize != hasQuantity || (hasSize && !DocSoKhongAm(fields, "sluong", soLuong))) {
            Gui(client, 400, "Bad Request", PhanHoi("error", "Size va so luong ton kho khong hop le."));
            return;
        }
        if (kho.KiemTraTrungMa(id)) {
            Gui(client, 409, "Conflict", PhanHoi("error", "ID san pham da ton tai."));
            return;
        }
        const string* img = fields.Tim("img");
        const string* desc = fields.Tim("desc");
        if (img != NULL && !img->empty() && !AnhHopLe(*img)) {
            Gui(client, 400, "Bad Request", PhanHoi("error", "Anh phai la URL HTTP/HTTPS hoac tep anh hop le."));
            return;
        }
        kho.ThemLoaiSanPham(id, *ten, *danhMuc, gia,
            img == NULL ? "" : *img, desc == NULL ? "" : *desc);
        if (hasSize && !kho.ThemKichCo(soLuong, *size, id)) {
            kho.XoaSanPham(id);
            Gui(client, 400, "Bad Request", PhanHoi("error", "Khong the tao size ton kho cho san pham."));
            return;
        }
    } else if (path == "/api/update-product") {
        int id;
        const string* ten = fields.Tim("ten");
        const string* danhMuc = fields.Tim("danhMuc");
        if (!DocSo(fields, "id", id) || ten == NULL || ten->empty() ||
            danhMuc == NULL || danhMuc->empty()) {
            Gui(client, 400, "Bad Request", PhanHoi("error", "Can nhap ID hop le, ten va danh muc san pham."));
            return;
        }
        if (!kho.CapNhatSanPham(id, *ten, *danhMuc)) {
            Gui(client, 404, "Not Found", PhanHoi("error", "Khong tim thay san pham."));
            return;
        }
    } else if (path == "/api/update-visibility") {
        int id, hienTrangChu, hienAoKhoac;
        if (!DocSo(fields, "id", id) ||
            !DocSoKhongAm(fields, "hienTrangChu", hienTrangChu) ||
            !DocSoKhongAm(fields, "hienAoKhoac", hienAoKhoac) ||
            (hienTrangChu != 0 && hienTrangChu != 1) ||
            (hienAoKhoac != 0 && hienAoKhoac != 1)) {
            Gui(client, 400, "Bad Request", PhanHoi("error", "Tuy chon hien thi khong hop le."));
            return;
        }
        if (!kho.CapNhatHienThi(id, hienTrangChu, hienAoKhoac)) {
            Gui(client, 404, "Not Found", PhanHoi("error", "Khong tim thay san pham."));
            return;
        }
    } else if (path == "/api/update-image") {
        int id;
        const string* hinhAnh = fields.Tim("img");
        if (!DocSo(fields, "id", id) || hinhAnh == NULL || !AnhHopLe(*hinhAnh)) {
            Gui(client, 400, "Bad Request", PhanHoi("error", "Anh khong hop le hoac vuot qua kich thuoc cho phep."));
            return;
        }
        if (!kho.CapNhatAnh(id, *hinhAnh)) {
            Gui(client, 404, "Not Found", PhanHoi("error", "Khong tim thay san pham."));
            return;
        }
    } else if (path == "/api/delete-product") {
        int id;
        if (!DocSo(fields, "id", id)) {
            Gui(client, 400, "Bad Request", PhanHoi("error", "Can nhap ID san pham hop le."));
            return;
        }
        if (!kho.XoaSanPham(id)) {
            Gui(client, 404, "Not Found", PhanHoi("error", "Khong tim thay san pham."));
            return;
        }
    } else if (path == "/api/import" || path == "/api/sell") {
        int id, quantity;
        const string* size = fields.Tim("size");
        if (!DocSo(fields, "id", id) || !DocSo(fields, "sluong", quantity) ||
            size == NULL || size->empty()) {
            Gui(client, 400, "Bad Request", PhanHoi("error", "Can nhap ID, size va so luong lon hon 0."));
            return;
        }
        if (!kho.KiemTraTrungMa(id)) {
            Gui(client, 404, "Not Found", PhanHoi("error", "Khong tim thay san pham."));
            return;
        }
        bool ok = path == "/api/import" ? kho.NhapThemSoLuong(id, *size, quantity) :
                  kho.BanSanPham(id, *size, quantity);
        if (!ok) {
            int stock = 0, sold = 0;
            bool found = kho.TimSanPham(id, *size, stock, sold);
            string error = path == "/api/import" ? "Khong the nhap hang: kiem tra size hoac so luong." :
                (found && stock < quantity ? "Khong du hang de ban. Ton kho hien tai: " :
                 "Khong tim thay size hoac so luong khong hop le.");
            if (path == "/api/sell" && found && stock < quantity) {
                ostringstream message;
                message << error << stock;
                error = message.str();
            }
            Gui(client, 400, "Bad Request", PhanHoi("error", error));
            return;
        }
    } else {
        Gui(client, 404, "Not Found", PhanHoi("error", "Khong tim thay API."));
        return;
    }
    if (!kho.LuuJSON(tenTep)) {
        NapKho(tenTep, kho);
        Gui(client, 500, "Internal Server Error", PhanHoi("error", "Khong the luu kho.json."));
        return;
    }
    Gui(client, 200, "OK", PhanHoi("success", path == "/api/delete-product" ?
        "Da xoa san pham." : path == "/api/add-product" ? "Da them san pham." :
        path == "/api/update-product" ? "Da cap nhat san pham." :
        path == "/api/update-visibility" ? "Da cap nhat vi tri hien thi." :
        path == "/api/update-image" ? "Da cap nhat anh san pham." :
        path == "/api/import" ? "Da nhap hang va cap nhat kho." : "Da ban hang va cap nhat kho."));
}

int main() {
    const string tenTep = "kho.json";
    KhoHang kho;
    if (!NapKho(tenTep, kho)) {
        cerr << "File kho.json khong dung dinh dang.\n";
        return 1;
    }
    WSADATA winsock;
    if (WSAStartup(MAKEWORD(2, 2), &winsock) != 0) {
        cerr << "Khong khoi dong duoc Winsock.\n";
        return 1;
    }
    SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server == INVALID_SOCKET) {
        cerr << "Khong tao duoc socket.\n";
        WSACleanup();
        return 1;
    }
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);
    if (bind(server, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == SOCKET_ERROR ||
        listen(server, SOMAXCONN) == SOCKET_ERROR) {
        cerr << "Khong mo duoc localhost:8080. Kiem tra cong co dang duoc su dung khong.\n";
        closesocket(server);
        WSACleanup();
        return 1;
    }
    cout << "API kho dang chay tai http://localhost:8080\n"
         << "Dang doc va ghi file " << tenTep << ". Nhan Ctrl+C de dung.\n";
    while (true) {
        SOCKET client = accept(server, NULL, NULL);
        if (client == INVALID_SOCKET) {
            cerr << "Loi nhan ket noi HTTP.\n";
            continue;
        }
        XuLy(client, kho, tenTep);
        shutdown(client, SD_SEND);
        closesocket(client);
    }
}