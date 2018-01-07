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
    
    // the number of encoder ticks to move per pixel
    private let ticksPerPixel : CGFloat = 1.0;

    // Maximum encoder ticks per second while coasting... if this gets too high
    // we'll end up with encoder wrap-around
    private let maxCoastSpeed : CGFloat = 1000;
    
    // time (in seconds) of the speed lowpass; this is just in place to make sure that
    // one sample alone doesn't define our coast speed, such as if the touchscreen
    // says the user slowed down when in fact he just eased off the screen
    private let speedLowpassTime = 0.2;
    
    private let minCoastSpeed : CGFloat = 0;
    private let coastDeceleration : CGFloat = 200;
    
    private let speedLog = NSMutableArray();

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
            
            // add our SpinnerLayer to show the spinner
            let spinnerLayer = SpinnerLayer();
            spinnerLayer.initialize();
            self.layer.addSublayer(spinnerLayer);
            
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
            speed = 0;
            speedLog.removeAllObjects();
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
            processMove(touch:touches.first!);
        }
        else
        {
            touchState = TouchState.NoTouch;
        }
    }
    
    override func touchesEnded(_ touches: Set<UITouch>, with event: UIEvent?) {
        if (touches.count == 1)
        {
            // first handle it as a regular move event
            processMove(touch:touches.first!);

            // and continue to coast at our current speed
            touchState = TouchState.Coasting;
            if (speed > maxCoastSpeed)
            {
                speed = maxCoastSpeed;
            }
            else if (speed < -maxCoastSpeed)
            {
                speed = -maxCoastSpeed;
            }
            coastDistance = 0;
        }
        else
        {
            touchState = TouchState.NoTouch;
        }
    }
    
    override func touchesCancelled(_ touches: Set<UITouch>, with event: UIEvent?) {
        touchState = TouchState.NoTouch;
    }
    
    private func processMove(touch:UITouch) {
        // move the spinner
        let xNow = touch.location(in: self).x;
        let totalChange = (Int32)(ticksPerPixel * (xNow - startX));
        cTempestMoveSpinner(
            tempest,
            totalChange - encoderChange
        );
        
        // calculate the speed of this single movement
        let x : CGFloat = touch.location(in: self).x;
        let time : CGFloat = CGFloat(touch.timestamp) - lastTouchTime;
        let speedThisTime = (x - lastX) / time;
        
        // and decay that into the speed
        speed += (speedThisTime - speed) * (1 - exp(-time / 0.1));
        speedLog.add(speed);
        
        lastX = touch.location(in: self).x;
        lastTouchTime = CGFloat(touch.timestamp);
        encoderChange = totalChange;
    }
}
