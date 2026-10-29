# MAISON - Website bán hàng và quản lý kho

## Mô tả đề tài

MAISON là website bán hàng thời trang kết hợp hệ thống quản lý kho. Khách hàng có thể xem sản phẩm, chọn kích cỡ, thêm vào giỏ hàng và đặt mua. Nhân viên có thể thêm, sửa, xóa sản phẩm; cập nhật ảnh và vị trí hiển thị; nhập hàng, ghi nhận bán hàng và kiểm tra tồn kho. Backend C++ quản lý sản phẩm bằng danh sách liên kết tự khai báo và lưu dữ liệu trong `kho2/kho.json`.

Giao diện sử dụng HTML, CSS và JavaScript; backend sử dụng C++ và Winsock để cung cấp API trên `localhost:8080`.

## Yêu cầu

- Windows
- Trình biên dịch C++ có hỗ trợ Winsock, ví dụ MinGW-w64/GCC
- Trình duyệt web
- Kết nối Internet để tải một số thư viện giao diện và ảnh từ CDN

## Biên dịch và chạy backend

Mở PowerShell tại thư mục dự án, sau đó chạy:

```powershell
cd D:\code\baitapweb\kho
g++ kho/server.cpp -o kho/server.exe -std=c++11 -lws2_32
.\kho\server.exe
```

Giữ cửa sổ server mở khi sử dụng website. Backend đọc và ghi `kho.json` trong thư mục hiện tại. Nếu cổng `8080` đang được sử dụng, hãy đóng chương trình đang chiếm cổng rồi chạy lại.

## Mở website

Mở thư mục `D:\code\baitapweb` bằng VS Code và dùng tiện ích **Live Server** để mở `index.html`. Các trang chính:

- `index.html`: Trang chủ
- `store.html`: Cửa hàng
- `aokhoac.html`: Sản phẩm áo khoác
- `kho2/index.html`: Quản lý kho

Backend phải đang chạy để các chức năng đọc và cập nhật dữ liệu kho hoạt động.
