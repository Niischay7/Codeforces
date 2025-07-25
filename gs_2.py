import cv2
import numpy as np
import math
import os

def get_intersection_points_count(image_path):
    img = cv2.imread(image_path)
    if img is None:
        return 0

    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    edges = cv2.Canny(gray, 50, 150, apertureSize=3)

    linesP = cv2.HoughLinesP(edges, rho=1, theta=np.pi / 180, threshold=30, minLineLength=20, maxLineGap=10)

    if linesP is None:
        return 0
    
    all_segments = [line[0] for line in linesP]

    horizontal_segments_info = []
    vertical_segments_info = []
    angle_tolerance_degrees = 2.5 

    for s_coords in all_segments:
        x1, y1, x2, y2 = s_coords
        if x1 == x2 and y1 == y2: continue
        
        angle_rad = np.arctan2(y2 - y1, x2 - x1)
        angle_deg = np.degrees(angle_rad)
        length = math.hypot(x2 - x1, y2 - y1)

        if abs(angle_deg) < angle_tolerance_degrees or abs(abs(angle_deg) - 180) < angle_tolerance_degrees:
            horizontal_segments_info.append({'segment': s_coords, 'length': length})
        elif abs(abs(angle_deg) - 90) < angle_tolerance_degrees:
            vertical_segments_info.append({'segment': s_coords, 'length': length})

    x_axis_repr_segment = None
    if horizontal_segments_info:
        horizontal_segments_info.sort(key=lambda x: x['length'], reverse=True)
        x_axis_repr_segment = horizontal_segments_info[0]['segment']

    y_axis_repr_segment = None
    if vertical_segments_info:
        vertical_segments_info.sort(key=lambda x: x['length'], reverse=True)
        y_axis_repr_segment = vertical_segments_info[0]['segment']

    found_intersections_coords = []
    point_on_line_tolerance = 1.5 

    def get_line_intersection_params(s1_coords, s2_coords):
        x1, y1, x2, y2 = s1_coords
        x3, y3, x4, y4 = s2_coords

        den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4)
        if den == 0:
            return None 

        t_num = (x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)
        u_num = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) # Corrected: was (x1-x3)

        # Check for near-zero denominator to avoid division by very small number if lines are almost parallel
        if abs(den) < 1e-9 : return None

        t = t_num / den
        u = u_num / den

        if 0.0 <= t <= 1.0 and 0.0 <= u <= 1.0:
            ix = x1 + t * (x2 - x1)
            iy = y1 + t * (y2 - y1)
            return ix, iy
        return None

    def is_point_on_infinite_line(px, py, line_seg_coords, tol):
        if line_seg_coords is None:
            return False
        x1, y1, x2, y2 = line_seg_coords
        if x1 == x2 and y1 == y2: # Segment is a point
             return math.hypot(px-x1, py-y1) < tol

        dist_numerator = abs((y2 - y1) * px - (x2 - x1) * py + x2 * y1 - y2 * x1)
        dist_denominator = math.hypot(y2 - y1, x2 - x1)
        
        if dist_denominator < 1e-9: # segment is effectively a point for line definition
             return math.hypot(px-x1, py-y1) < tol

        distance = dist_numerator / dist_denominator
        return distance < tol

    for i in range(len(all_segments)):
        for j in range(i + 1, len(all_segments)):
            s1 = all_segments[i]
            s2 = all_segments[j]
            
            intersect_pt = get_line_intersection_params(s1, s2)

            if intersect_pt is not None:
                ix, iy = intersect_pt
                
                on_identified_x_axis = is_point_on_infinite_line(ix, iy, x_axis_repr_segment, point_on_line_tolerance)
                on_identified_y_axis = is_point_on_infinite_line(ix, iy, y_axis_repr_segment, point_on_line_tolerance)
                
                if not (on_identified_x_axis or on_identified_y_axis):
                    found_intersections_coords.append((round(ix), round(iy)))
    
    if not found_intersections_coords:
        return 0

    unique_points = []
    cluster_radius_sq = (3.0)**2 

    for p_coords in found_intersections_coords:
        px, py = p_coords 
        is_unique = True
        for up_coords in unique_points:
            upx, upy = up_coords
            dist_sq = (px - upx)**2 + (py - upy)**2
            if dist_sq < cluster_radius_sq:
                is_unique = False
                break
        if is_unique:
            unique_points.append((px, py))
            
    return len(unique_points)

if __name__ == '__main__':
    image_filename = input()
    # As per problem guidelines, assume 'image_filename' is a valid path that can be read.
    # os.path.isfile could be used here, but cv2.imread handles file not found by returning None,
    # and the function get_intersection_points_count handles img is None.
    count = get_intersection_points_count(image_filename)
    print(count)