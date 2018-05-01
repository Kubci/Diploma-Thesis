function result = imposemin(image, mask)
% imposemin(f, m)
% Impose minima into image f in non-zero pixels of marker m
% 
% Petr Matula (pem@fi.muni.cz)

t_max = max(image(:)) + 2;

marker = t_max * (mask == 0);

result = -reconstruction(-marker, -min(marker, image + 1));