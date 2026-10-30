const INVENTORY_API_URL = 'http://localhost:8080/api/products';

async function refreshInventory() {
    try {
        const response = await fetch(INVENTORY_API_URL);
        if (!response.ok) throw new Error(`HTTP ${response.status}`);

        const data = await response.json();
        const inventoryProducts = Array.isArray(data) ? data : data.san_pham;
        if (!Array.isArray(inventoryProducts)) throw new Error('Dữ liệu kho không đúng định dạng.');

        const inventory = new Map(inventoryProducts.map(product => [
            String(product.id),
            {
                ...product,
                stock: (product.sizes || []).reduce((total, size) => total + Number(size.so_luong || 0), 0)
            }
        ]));
        window.inventoryCatalog = inventory;

        if (typeof products !== 'undefined' && typeof renderProducts === 'function') {
            const productIds = new Set(products.map(item => String(item.inventoryId)).filter(id => id !== 'undefined'));
            inventory.forEach(product => {
                if (productIds.has(String(product.id))) return;
                products.push({
                    id: Number(product.id),
                    inventoryId: product.id,
                    name: product.ten,
                    price: Number(product.gia || 0),
                    img: product.img || '',
                    desc: product.desc || ''
                });
                productIds.add(String(product.id));
            });
            products = products.filter(item => inventory.has(String(item.inventoryId)));
            products.forEach(item => {
                const product = inventory.get(String(item.inventoryId));
                item.name = product.ten;
                if (Number(product.gia) > 0) item.price = Number(product.gia);
                if (product.img) item.img = product.img;
                if (product.desc) item.desc = product.desc;
            });
            renderProducts();
        }

        document.querySelectorAll('[data-stock-id]').forEach(card => {
            const product = inventory.get(card.dataset.stockId);
            const visibilityField = document.body.dataset.inventorySection === 'home'
                ? 'hienTrangChu'
                : document.body.dataset.inventorySection === 'jacket' ? 'hienAoKhoac' : '';
            const isVisible = product && (!visibilityField || Number(product[visibilityField]) !== 0);
            card.hidden = !isVisible;
            card.style.display = isVisible ? '' : 'none';
            if (!product) return;

            const heading = card.querySelector('h2, h3, h4');
            if (heading) heading.textContent = product.ten;
            card.querySelectorAll('img').forEach(image => {
                image.alt = product.ten;
            });

            let indicator = card.querySelector('[data-stock-indicator]');
            if (!indicator) {
                indicator = document.createElement('p');
                indicator.dataset.stockIndicator = '';
                indicator.className = 'text-xs text-apple-subtext mt-2';
                if (heading) heading.insertAdjacentElement('afterend', indicator);
                else card.prepend(indicator);
            }

            const sizes = (product.sizes || [])
                .map(size => `${size.size}: ${size.so_luong}`)
                .join(', ');
            const category = product.danhMuc && product.danhMuc !== 'Chua phan loai'
                ? `${product.danhMuc} · `
                : '';
            indicator.textContent = `${category}Còn ${product.stock} sản phẩm${sizes ? ` (${sizes})` : ''}`;
            indicator.classList.toggle('text-red-400', product.stock === 0);
            indicator.classList.toggle('text-emerald-400', product.stock > 0);

            card.querySelectorAll('button[onclick*="addToCart"], button[data-inventory-add]').forEach(button => {
                button.disabled = product.stock <= 0;
                button.classList.toggle('opacity-50', product.stock <= 0);
                button.classList.toggle('cursor-not-allowed', product.stock <= 0);
                button.title = product.stock <= 0 ? 'Sản phẩm hiện đã hết hàng' : '';
            });
        });

        document.dispatchEvent(new CustomEvent('inventory:loaded', { detail: inventory }));
    } catch (error) {
        document.querySelectorAll('[data-stock-id]').forEach(card => {
            let indicator = card.querySelector('[data-stock-indicator]');
            if (!indicator) {
                indicator = document.createElement('p');
                indicator.dataset.stockIndicator = '';
                indicator.className = 'text-xs text-amber-400 mt-2';
                const heading = card.querySelector('h2, h3, h4');
                if (heading) heading.insertAdjacentElement('afterend', indicator);
                else card.prepend(indicator);
            }
            indicator.textContent = 'Không thể tải tồn kho';
        });
        console.error('Không thể tải tồn kho:', error);
    }
}

document.addEventListener('click', event => {
    const button = event.target instanceof Element
        ? event.target.closest('button[onclick*="addToCart"], button[data-inventory-add]')
        : null;
    const card = button?.closest('[data-stock-id]');
    const product = card && window.inventoryCatalog?.get(card.dataset.stockId);
    if (!button || !card || !product) return;

    event.preventDefault();
    event.stopImmediatePropagation();
    const priceMatch = button.getAttribute('onclick')?.match(/addToCart\(\s*['"][^'"]*['"]\s*,\s*(\d+)/);
    const price = button.dataset.price !== undefined
        ? Number(button.dataset.price)
        : priceMatch ? Number(priceMatch[1]) : 0;
    const size = product.sizes?.find(item => Number(item.so_luong) > 0)?.size || '';
    window.addToCart(product.ten, price, size, product.id, size);
}, true);

window.addEventListener('DOMContentLoaded', refreshInventory);
window.addEventListener('focus', refreshInventory);
