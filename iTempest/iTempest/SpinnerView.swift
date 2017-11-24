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
    private var tempest : cTempest = 0;
    private var initialized : Bool = false;
    private var startX : CGFloat = 0;
    private var encoderChange: Int32 = 0;
    private var sweeping : Bool = false;
    
    func setTempest(tempest:cTempest) {
        if (!initialized) {
            backgroundColor = UIColor.blue;
            isUserInteractionEnabled = true;
            
            initialized = true;
        }
        self.tempest = tempest;
    }

    override func touchesBegan(_ touches: Set<UITouch>, with event: UIEvent?) {
        // only single touches
        if (touches.count == 1)
        {
            startX = touches.first!.location(in: self).x;
            encoderChange = 0;
            sweeping = true;
        }
        else
        {
            sweeping = false;
        }
    }
    
    override func touchesMoved(_ touches: Set<UITouch>, with event: UIEvent?) {
        // never mind if we don't have a start position
        if (!sweeping)
        {
            return;
        }
        
        // only single touches
        if (touches.count == 1)
        {
            let xNow = touches.first!.location(in: self).x;
            let totalChange = (Int32)((1.0) * (xNow - startX));
            cTempestMoveSpinner(
                tempest,
                totalChange - encoderChange
            );
            encoderChange = totalChange;
        }
        else
        {
            sweeping = false;
        }
    }
    
    override func touchesEnded(_ touches: Set<UITouch>, with event: UIEvent?) {
        sweeping = false;
    }
    
    override func touchesCancelled(_ touches: Set<UITouch>, with event: UIEvent?) {
        sweeping = false;
    }
}
