// Hàm chuyển trang và vẽ lại giao diện
const navigateTo = url => {
    history.pushState(null, null, url);
    router();
};

const router = async () => {
    // Lấy đường dẫn URL hiện tại
    const path = window.location.pathname;
    
    // Tìm trang tương ứng trong routes, nếu không có thì trả về 404
    const route = routes[path] || routes["404"];

    // Cập nhật thẻ <title> của trình duyệt
    document.title = route.title;

    // Render HTML vào thẻ <main id="app">
    document.getElementById("app").innerHTML = route.render();
};

// Bắt sự kiện khi người dùng bấm nút Back/Forward trên trình duyệt
window.addEventListener("popstate", router);

// Bắt sự kiện click vào các liên kết có thuộc tính [data-link]
document.addEventListener("DOMContentLoaded", () => {
    document.body.addEventListener("click", e => {
        if (e.target.matches("[data-link]")) {
            e.preventDefault(); // Ngăn trình duyệt tải lại trang kiểu truyền thống
            navigateTo(e.target.href);
        }
    });

    // Chạy router lần đầu tiên khi mở trang
    router();
});