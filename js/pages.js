// Khai báo giao diện cho từng đường dẫn (Route)
const routes = {
    "/": {
        title: "Apple Fashion - Trang Chủ",
        render: () => `
            <section class="h-[90vh] bg-black text-white flex flex-col justify-center items-center text-center px-4">
                <h1 class="text-5xl md:text-7xl font-semibold tracking-tight mb-2">BST Mùa Thu 2026</h1>
                <p class="text-xl md:text-2xl text-gray-400 mb-6">Thiết kế tối giản. Chất liệu thượng hạng.</p>
                <div class="space-x-4">
                    <a href="/store" data-link class="bg-[#0071e3] text-white px-5 py-2 rounded-full text-sm font-medium hover:bg-[#0077ed]">Mua ngay</a>
                    <a href="/ao" data-link class="text-[#2997ff] hover:underline text-sm font-medium">Tìm hiểu thêm ></a>
                </div>
            </section>
        `
    },
    "/store": {
        title: "Cửa Hàng - Apple Fashion",
        render: () => `
            <section class="max-w-[1024px] mx-auto py-12 px-4">
                <h1 class="text-4xl font-semibold mb-2">Cửa Hàng. <span class="text-gray-500">Cách tốt nhất để mua sắm sản phẩm bạn yêu thích.</span></h1>
                <div class="grid grid-cols-1 md:grid-cols-3 gap-6 mt-8">
                    <div class="bg-white p-6 rounded-2xl shadow-sm border border-gray-200">
                        <h3 class="text-xl font-semibold mb-2">Áo Sơ Mi Silk</h3>
                        <p class="text-gray-500 mb-4">2.490.000đ</p>
                        <a href="/ao" data-link class="text-[#0071e3] hover:underline">Xem chi tiết ></a>
                    </div>
                    <div class="bg-white p-6 rounded-2xl shadow-sm border border-gray-200">
                        <h3 class="text-xl font-semibold mb-2">Quần Trouser Slim</h3>
                        <p class="text-gray-500 mb-4">1.890.000đ</p>
                        <a href="/quan" data-link class="text-[#0071e3] hover:underline">Xem chi tiết ></a>
                    </div>
                </div>
            </section>
        `
    },
    "/ao": {
        title: "Bộ Sưu Tập Áo - Apple Fashion",
        render: () => `
            <section class="max-w-[1024px] mx-auto py-12 px-4">
                <h1 class="text-3xl font-semibold mb-6">Bộ Sưu Tập Áo Nam & Nữ</h1>
                <p class="text-gray-600">Đang cập nhật danh mục sản phẩm chi tiết...</p>
            </section>
        `
    },
    "/admin": {
        title: "Quản Lý Hệ Thống",
        render: () => `
            <section class="max-w-[1024px] mx-auto py-12 px-4">
                <h1 class="text-3xl font-semibold mb-6">Hệ Thống Quản Lý (Admin)</h1>
                <p class="text-gray-600">Trang quản lý tồn kho và đơn hàng.</p>
            </section>
        `
    },
    "404": {
        title: "Trang Không Tồn Tại",
        render: () => `
            <section class="max-w-[1024px] mx-auto py-24 px-4 text-center">
                <h1 class="text-4xl font-semibold mb-4">404 - Trang không tìm thấy</h1>
                <a href="/" data-link class="text-[#0071e3] hover:underline">Trở về trang chủ</a>
            </section>
        `
    }
};