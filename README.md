# BÀI TẬP LỚN: WEBSITE THỜI TRANG MAISON VÀ QUẢN LÝ KHO

> **Môn học:** [Cấu Trúc Dữ Liệu Và Giải Thuật]<br>
> **Lớp:** [CQ.66.KT-ĐTVT]<br>
> **Nhóm thực hiện:** [Nhóm 12]

---

## 👥 Danh sách Thành viên

| STT | Họ và Tên | MSSV | Vai trò / Nhiệm vụ | GitHub Username |
| 1 | [Đặng Duy Tùy Kê]         | [6651020030] | [Nhóm Trưởng/Thiết kế và triển khai cấu trúc,UI] | [@username](https://github.com/) |

| 2 | [Trương Phạm Gia Huy]     | [6651020028] | [Thành viên/TT Quản lý danh sách sản phẩm]       | [@username](https://github.com/) |

| 3 | [Võ Đình Khải]            | [6651020031] | [Thành viên/TT Quản lý kích cỡ và tồn kho]       | [@username](https://github.com/) |

| 4 | [Hoàng Phi Hùng]          | [6651020024] | [Thành viên/TT Thống kê và kiểm thử]             | [@username](https://github.com/) |

| 5 | [Lê Nguyễn Ngọc Bảo Châu] | [6651020030] | [Thành viên/TT Tìm kiếm và lọc sản phẩm]         | [@username](https://github.com/) |

---

## 📌 Giới thiệu Đề tài

MAISON là website thời trang kết hợp với chương trình quản lý kho. Website cung cấp giao diện giới thiệu và trưng bày sản phẩm; nhân viên có thể quản lý sản phẩm và theo dõi số lượng tồn kho theo kích cỡ.

Các tính năng chính:

- Hiển thị sản phẩm trên trang chủ, cửa hàng và trang áo khoác; hỗ trợ tìm kiếm, giỏ hàng và chuyển đổi giao diện sáng/tối.
- Quản lý kho: xem danh sách và thống kê sản phẩm, lọc theo danh mục, thêm/cập nhật/xóa sản phẩm, cập nhật ảnh và vị trí hiển thị.
- Nhập hàng và ghi nhận bán hàng theo sản phẩm, kích cỡ; theo dõi số lượng tồn và số lượng đã bán.
- Lưu dữ liệu kho trong file JSON để dùng chung giữa website và chương trình quản lý kho.

> Đăng nhập trên website hiện chỉ lưu trạng thái trên thiết bị, chưa xác thực tài khoản qua máy chủ.

---

## 🛠️ Yêu cầu Hệ thống & Môi trường

- **Hệ điều hành:** Windows (backend sử dụng Winsock).
- **Trình biên dịch:** MinGW-w64/GCC có hỗ trợ C++11 và Winsock.
- **Trình duyệt:** Chrome, Edge, Firefox hoặc trình duyệt hiện đại tương đương.
- **Công cụ khuyến nghị:** Visual Studio Code và tiện ích Live Server.
- **Kết nối Internet:** Cần thiết để tải Tailwind CSS, biểu tượng và một số tài nguyên giao diện từ CDN.

---

## 🚀 Hướng dẫn Cài đặt & Chạy

### 1. Tải dự án về máy

```bash
git clone https://github.com/GTMMC/BTL.git
cd BTL
```

### 2. Biên dịch và chạy backend quản lý kho

Mở PowerShell tại thư mục dự án, sau đó chạy:

```powershell
cd D:\code\baitapweb\kho
g++ -std=c++11 server.cpp -o server.exe -lws2_32
.\server.exe
```

Giữ cửa sổ terminal đang chạy server. Server cung cấp API tại `http://localhost:8080` và đọc/ghi dữ liệu trong `kho\kho.json` theo thư mục làm việc hiện tại. Vì vậy, hãy chạy `server.exe` từ thư mục `kho`. Nếu cổng `8080` đang được sử dụng, hãy đóng chương trình đang chiếm cổng rồi khởi động lại server.

### 3. Mở giao diện website

Mở thư mục dự án bằng VS Code, dùng **Live Server** để mở `index.html`. Các trang chính:

- `index.html`: Trang chủ.
- `store.html`: Trang cửa hàng.
- `aokhoac.html`: Trang sản phẩm áo khoác.
- `kho/index.html`: Giao diện quản lý kho.

Backend cần hoạt động để website tải số lượng tồn và để các chức năng cập nhật kho được lưu vào file JSON.
