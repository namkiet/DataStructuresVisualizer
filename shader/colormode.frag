uniform sampler2D texture; // Ảnh đầu vào (hình vẽ)
void main() {
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy); // Lấy màu của pixel hiện tại
    
    // Nếu pixel có màu đỏ (giá trị R cao, G và B thấp)
    if (pixel.r > 0.9 && pixel.g < 0.1 && pixel.b < 0.1) {
        pixel.rgb = vec3(0.0, 0.0, 1.0); // Chuyển thành xanh (R=0, G=0, B=1)
    }
    
    gl_FragColor = pixel; // Xuất màu sau khi xử lý
}
