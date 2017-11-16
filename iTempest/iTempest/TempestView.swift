//
//  TempestView.swift
//  iTempest
//
//  Created by Randy Rasmussen on 11/15/17.
//  Copyright © 2017 Randy Rasmussen. All rights reserved.
//

import Foundation
import UIKit

class TempestView : UIView {
    private var initialized : Bool = false;
    private let lines : NSMutableArray = NSMutableArray();
    private let paths : NSMutableArray = NSMutableArray();

    public func showVectors(vectors:cVectors)
    {
        if (!initialized)
        {
            // create our arrays of screen objects
            for _ in 0..<1000
            {
                let path : UIBezierPath = UIBezierPath();
                paths.add(path);
                
                let line : CAShapeLayer = CAShapeLayer();
                self.layer.addSublayer(line);
                lines.add(line);
            }
            
            initialized = true;
        }
        
        self.backgroundColor = UIColor.black;
        
        var startX : Int16 = 0;
        var endX : Int16 = 0;
        var startY : Int16 = 0;
        var endY : Int16 = 0;
        var r : UInt8 = 0;
        var g : UInt8 = 0;
        var b : UInt8 = 0;
        
        let scale : CGFloat = 64 * 1024;
        let width = self.frame.size.width;
        let height = self.frame.size.height;
        let size = width < height ? width : height;

        // draw lines
        var i : Int = 0;
        while (0 != cVectorsGetNext(vectors, &startX, &startY, &endX, &endY, &r, &g, &b))
        {
            if (i >= lines.count) {
                break;
            }
            
            // get the current path and clear it
            let path : UIBezierPath = paths.object(at:i) as! UIBezierPath;
            path.removeAllPoints();
            
            // tempest sends a zero length vector to indicate a dot; deal with that
            if (startX==endX && startY==endY)
            {
                path.move(to: CGPoint(x: size * (0.5 + CGFloat(startX)/scale) - 0.5, y:size * (0.5 - CGFloat(startY)/scale)));
                path.addLine(to: CGPoint(x:size * (0.5 + CGFloat(endX)/scale) + 0.5, y:size * (0.5 - CGFloat(endY)/scale)));
            }
            else
            {
                path.move(to: CGPoint(x: size * (0.5 + CGFloat(startX)/scale), y:size * (0.5 - CGFloat(startY)/scale)));
                path.addLine(to: CGPoint(x:size * (0.5 + CGFloat(endX)/scale), y:size * (0.5 - CGFloat(endY)/scale)));
            }
            
            let layer : CAShapeLayer = lines.object(at:i) as! CAShapeLayer;
            layer.path = path.cgPath;
            layer.opacity = 1;
            layer.strokeColor = UIColor.init(red:CGFloat(r)/255.0, green:CGFloat(g)/255.0 ,blue:CGFloat(b)/255.0, alpha:1).cgColor;
            
            i = i + 1;
        }
        
        // hide the rest of the lines in the array
        while (i < lines.count)
        {
            let layer : CAShapeLayer = lines.object(at:i) as! CAShapeLayer;
            layer.opacity = 0;
            i = i + 1;
        }
    }
}

