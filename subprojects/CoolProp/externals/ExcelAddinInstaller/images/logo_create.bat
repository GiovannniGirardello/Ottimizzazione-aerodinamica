convert logo_large.png -gravity center -background white -flatten -resize 150x150 -extent 150x150 logo.bmp
convert logo_large.png -gravity center -background white -flatten -resize 55x55 -extent 55x55 logo_small.bmp
convert logo_large.png -background white -flatten -bordercolor white -border 0 ( -clone 0 -resize 16x16 ) ( -clone 0 -resize 32x32 ) ( -clone 0 -resize 48x48 ) ( -clone 0 -resize 64x64 ) -delete 0 -alpha off -colors 256 "logo.ico"