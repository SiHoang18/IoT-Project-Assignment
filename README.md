# HƯỚNG DẪN CÀI ĐẶT VÀ SỬ DỤNG DỰ ÁN (PLATFORMIO)

Tài liệu này cung cấp quy trình chi tiết dành cho kỹ sư và nhà nghiên cứu để thiết lập môi trường, biên dịch và nạp mã nguồn vào thiết bị nhúng sử dụng nền tảng PlatformIO trên Visual Studio Code.

## 1. Thông tin phần cứng

Dự án được cấu hình và kiểm thử dựa trên nền tảng phần cứng sau:

* **Tên Bo mạch:** [Nhập tên bo mạch của bạn tại đây]
* **Link chi tiết/Mua hàng:** [ĐÍNH KÈM LINK BO MẠCH CỦA BẠN TẠI ĐÂY]

## 2. Yêu cầu phần mềm

Trước khi thực hiện, máy tính của bạn cần được cài đặt các công cụ sau:

1.  **Visual Studio Code (VSCode):** Trình soạn thảo mã nguồn chính.
2.  **Git:** Công cụ quản lý phiên bản để tải mã nguồn.
3.  **USB Driver:** Đảm bảo đã cài đặt Driver tương ứng với chip giao tiếp Serial trên bo mạch (ví dụ: CP210x, CH340, hoặc FTDI).

## 3. Cài đặt Extension PlatformIO

Để làm việc với các dự án nhúng trong VSCode, bạn cần cài đặt Extension PlatformIO IDE:

1.  Mở **Visual Studio Code**.
2.  Nhấn vào biểu tượng **Extensions** ở thanh bên trái (hoặc nhấn tổ hợp phím `Ctrl + Shift + X`).
3.  Tìm kiếm từ khóa `PlatformIO IDE`.
4.  Nhấn nút **Install**. 
5.  Sau khi cài đặt, bạn sẽ thấy biểu tượng hình **Kiến (Ant)** xuất hiện ở thanh công cụ bên trái. Đợi hệ thống hoàn tất cấu hình ban đầu (có thể mất vài phút).

## 4. Triển khai mã nguồn

### Bước 1: Clone dự án về máy

Mở Terminal (hoặc Command Prompt) và chạy lệnh sau để tải dự án:

```bash
git clone <URL_CUA_DU_AN_TAI_DAY>
### Bước 2: Mở dự án trong môi trường làm việc
1. Trong VSCode, chọn **File > Open Folder...**
2. Tìm và chọn thư mục dự án bạn vừa tải về.
3. PlatformIO sẽ tự động nhận diện tệp cấu hình `platformio.ini`. Hệ thống sẽ tự động tải xuống các thư viện và công cụ biên dịch (Toolchains) cần thiết dựa trên tệp này.

## 5. Quy trình Biên dịch và Nạp Code (Build & Upload)
Các thao tác chính nằm ở thanh trạng thái (Status Bar) phía dưới cùng của cửa sổ VSCode:

* **Build (Biên dịch):**
  * Nhấn vào biểu tượng **Dấu tích (✓)**.
  * Quá trình này sẽ kiểm tra lỗi cú pháp và tạo tệp firmware. Kết quả cuối cùng phải hiển thị dòng chữ `SUCCESS` màu xanh trong Terminal.

* **Upload (Nạp code):**
  * Kết nối bo mạch của bạn với máy tính thông qua cáp USB.
  * Nhấn vào biểu tượng **Mũi tên sang phải (→)**.
  * PlatformIO sẽ tự động tìm kiếm cổng COM và nạp chương trình vào bo mạch.

* **Serial Monitor (Theo dõi phản hồi):**
  * Nhấn vào biểu tượng **Hình ổ cắm (Plug)** để theo dõi các thông báo từ bo mạch gửi lên máy tính qua cổng Serial. Lưu ý kiểm tra tốc độ Baudrate trong tệp `platformio.ini` để đảm bảo hiển thị đúng ký tự.