//
//  SpinnerView.swift
//  iTempest
//
//  Created by Randy Rasmussen on 11/21/17.
//  Copyright © 2017 Randy Rasmussen. All rights reserved.
//

import Foundation
import UIKit

class SpinnerView : UIView
{
    private enum TouchState {
        case NoTouch
        case Dragging
        case Coasting
    };
    
    private var touchState : TouchState = TouchState.NoTouch;
    
    private var tempest : cTempest = 0;
    private var initialized : Bool = false;
    private var encoderChange: Int32 = 0;

    private var timer : Timer? = nil;
    
    private var startX : CGFloat = 0;
    private var lastX : CGFloat = 0;
    private var lastTouchTime : CGFloat = 0;
    private var speed : CGFloat = 0;
    private var coastDistance : CGFloat = 0;
    
    private let ticksPerPixel : CGFloat = 1.0;
    private let maxCoastSpeed : CGFloat = 400;
    private let minCoastSpeed : CGFloat = 100;
    private let coastDeceleration : CGFloat = 100;

    func setTempest(tempest:cTempest) {
        if (!initialized) {
            backgroundColor = UIColor.blue;
            isUserInteractionEnabled = true;
            
            timer = Timer.scheduledTimer(
                timeInterval: 0.01,
                target: self,
                selector: #selector(swipeTimer),
                userInfo: nil,
                repeats: true);
            
            initialized = true;
        }
        self.tempest = tempest;
    }

    func swipeTimer() {
        if (touchState != TouchState.Coasting)
        {
            return;
        }
        
        // apply some static friction, otherwise the spinner creeps forever
        if (speed <= minCoastSpeed && speed >= -minCoastSpeed)
        {
            touchState = TouchState.NoTouch;
            return;
        }
        
        // add to the distance that we coasted
        coastDistance += 0.01 * speed;
        while (coastDistance >= 1) {
            coastDistance -= 1;
            cTempestMoveSpinner(tempest, 1);
        }
        while (coastDistance <= -1) {
            coastDistance += 1;
            cTempestMoveSpinner(tempest, -1);
        }

        // friction
        if (speed > 0)
        {
            speed -= 0.01 * coastDeceleration;
            if (speed < 0)
            {
                speed = 0;
            }
        }
        else
        {
            speed += 0.01 * coastDeceleration;
            if (speed > 0)
            {
                speed = 0;
            }
        }
    }
    
    override func touchesBegan(_ touches: Set<UITouch>, with event: UIEvent?) {
        // only single touches
        if (touches.count == 1)
        {
            startX = touches.first!.location(in: self).x;
            lastX = startX;
            lastTouchTime = CGFloat(touches.first!.timestamp);
            encoderChange = 0;
            touchState = TouchState.Dragging;
        }
        else
        {
            touchState = TouchState.NoTouch;
        }
    }
    
    override func touchesMoved(_ touches: Set<UITouch>, with event: UIEvent?) {
        // never mind if we don't have a start position
        if (touchState != TouchState.Dragging)
        {
            return;
        }
        
        // only single touches
        if (touches.count == 1)
        {
            let xNow = touches.first!.location(in: self).x;
            let totalChange = (Int32)(ticksPerPixel * (xNow - startX));
            cTempestMoveSpinner(
                tempest,
                totalChange - encoderChange
            );
            
            let x : CGFloat = touches.first!.location(in: self).x;
            let time : CGFloat = CGFloat(touches.first!.timestamp);
            speed = (x - lastX) / CGFloat(time - lastTouchTime);
            lastX = touches.first!.location(in: self).x;
            lastTouchTime = CGFloat(touches.first!.timestamp);
            encoderChange = totalChange;
        }
        else
        {
            touchState = TouchState.NoTouch;
        }
    }
    
    override func touchesEnded(_ touches: Set<UITouch>, with event: UIEvent?) {
        let x : CGFloat = touches.first!.location(in: self).x;
        let time : CGFloat = CGFloat(touches.first!.timestamp);
        if (time != lastTouchTime)
        {
            speed = ticksPerPixel * (x - lastX) / (time - lastTouchTime);
            if (speed > maxCoastSpeed)
            {
                speed = maxCoastSpeed;
            }
            else if (speed < -maxCoastSpeed)
            {
                speed = -maxCoastSpeed;
            }
        }
        
        touchState = TouchState.Coasting;
        coastDistance = 0;
    }
    
    override func touchesCancelled(_ touches: Set<UITouch>, with event: UIEvent?) {
        touchState = TouchState.NoTouch;
    }
}
