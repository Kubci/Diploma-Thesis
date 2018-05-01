folder = 'D:\_SKOLA\_Master\_Diplomka\Data\vyhodnotenie GT\QPI\';

for i = 1 : 18

cout = sprintf( '%02d', i);

image = double(imread(strcat(folder, cout, '.tif')));
detection = imread(strcat(folder, 'res3\', cout, '_singleJ.png'));
gt = imread(strcat(folder, 'GT\', cout, '_gt.tif'));
maxJ = imread(strcat(folder, 'res3\', cout, '_globalJ.png'));

max_i = max(max(image));
min_i = min(min(image));

image = 255 * ( (image - min_i) / (max_i - min_i) );
image_i = 255 - image; 

detection = detection > 0;
gt = gt > 0;
maxJ = maxJ > 0;

addpath(fileparts(which('imdilate')));

det_e = erosion(detection, 9, 'elliptic');
backMarker_e = erosion(~maxJ, 9, 'elliptic');
markers = det_e | backMarker_e;
markers = bwareaopen(logical(markers),15);
ws_image = imposemin(image_i, markers);

res = watershed(double(ws_image));
res_m = res > 0;

over = overlay(image, res_m - erosion(res_m, 3, 'rectangular'));
gtOver = overlay(gt, res_m - erosion(res_m, 3, 'rectangular'));

imwrite(uint8(over), strcat(folder, 'segmentation\',cout, '_segOver.png'));
imwrite(res_m, strcat(folder, 'segmentation\',cout, '_segMask.png'));
imwrite(uint8(gtOver), strcat(folder, 'segmentation\',cout, '_segOverGgt.png'));

end