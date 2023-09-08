import matplotlib.pylab as plt
import cv2
import numpy as np
from object_detection import ObjectDetection
import math




# load object detection
od = ObjectDetection()

cap = cv2.VideoCapture('10.mp4')
out = cv2.VideoWriter('output.mp4', -1, 20.0, (720, 1280))

count = 0
vcount = 0
area = [(385, 676), (595, 120), (395, 676), (605, 120)]
region_id = set()
center_pre_frame = []
tracking_id = 0
tracking_obj = {}
# center_pt =[]
# Read until video is completed
while cap.isOpened():
    ret, frame = cap.read()
    height, width, _ = frame.shape
    frame1 = frame.copy()
    count += 1
    if not ret:
        break
    # vcount +=1
    # if vcount %3 !=0:
    # continue
    center_cur_frame = []

    # detect obj on frame

    (class_id, scores, boxes) = od.detect(frame1)
    for box in boxes:
        (x, y, w, h) = box
        cx = int((x + x + w) / 2)
        cy = int((y + y + w) / 2)
        center_cur_frame.append((cx, cy,x, y ,w , h))
        print("frame no.", count, " BOX ", x, y, w, h)

        cv2.rectangle(frame1, (x, y), (x + w, y + h), (0, 255, 0), 2)


    if count <= 2:
        for pt in center_cur_frame:
            for pt2 in center_pre_frame:
                distance = math.hypot(pt2[0] - pt[0], pt2[1] - pt[1])
                if distance < 20:
                    tracking_obj[tracking_id] = pt
                    tracking_id += 1
    else:
        tracking_obj_copy = tracking_obj.copy()
        center_cur_frame_copy = center_cur_frame.copy()
        for object_id, pt2 in tracking_obj_copy.items():
            obj_exists = False
            for pt in center_cur_frame_copy:
                distance = math.hypot(pt2[0] - pt[0], pt2[1] - pt[1])
                if distance < 15:
                    tracking_obj[object_id] = pt
                    obj_exists = True
                    if pt in center_cur_frame:
                        center_cur_frame.remove(pt)
                    continue
            if not obj_exists:
                tracking_obj.pop(object_id)

        for pt in center_cur_frame:
            tracking_obj[tracking_id] = pt
            tracking_id += 1

            # cv2.circle(frame,pt,5,(0,0,255),-1)
    cv2.line(frame1, (380, 676), (595, 120), (0, 255, 0), 2)
    cv2.line(frame1, (395, 676), (605, 120), (0, 255, 0), 2)

    for object_id, pt in tracking_obj.items():
        cx, cy, x, y, w, h = pt

        cv2.circle(frame1, ((cx),(cy)), 5, (0, 0, 255), -1)

        cv2.putText(frame1, str(object_id), (pt[0], pt[1] - 7), 0, 1, (0, 0, 255), 2)

        print(cx, " ", cy)
        inside_region = cv2.pointPolygonTest(np.array(area), (int(cx-12), int(cy+50)), False)
        #cv2.circle(frame1, ((cx-12),(cy+50)), 5, (0, 0, 255), -1)
        print(inside_region)
        if inside_region > 0:
            region_id.add(object_id)
        #for box in boxes:
            #(x1, y1, w, h) = box
            #cx = int((x1 + x1 + w) / 2)
            #cy = int((y1 + y1 + w) / 2)
            #cx == int(x) and cy == int(y) and
        if  object_id in region_id :
            cv2.rectangle(frame1, (x, y), (x + w, y + h), (0, 0 , 255), 2)
        print(region_id)
    print(tracking_obj)
    vehicle_count = len(region_id)
    cv2.putText(frame1, "Violation:" + str(vehicle_count), (10, 100), 0, 1.5, (0, 0, 0), 2)

    cv2.imshow('frame1', frame1)

    if ret == True:
        out.write(frame1)

        # write the flipped frame

    # cv2.imshow('frame', frame)
    # key=cv2.waitKey(1)
    # if key == 10:
    # break
    # make copy center_pt
    center_pre_frame = center_cur_frame.copy()
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
out.release()
cv2.destroyAllWindows()