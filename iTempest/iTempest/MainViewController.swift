//
//  MainViewController.swift
//  iTempest
//
//  Created by Randy Rasmussen on 11/14/17.
//  Copyright © 2017 Randy Rasmussen. All rights reserved.
//

import UIKit

class MainViewController: UIViewController {
    var tempestView : TempestView = TempestView();
    var tempest : cTempest = cTempestCreate();
    var p1Button : UIButton = UIButton(type: UIButtonType.roundedRect);

    override func viewDidLoad() {
        // call the super
        super.viewDidLoad();
        
        // set our background
        self.view.layer.backgroundColor = UIColor.brown.cgColor;
        
        // initialize our subviews
        p1Button.layer.cornerRadius = 40;
        p1Button.backgroundColor = UIColor.red;
        
        // add our subviews
        self.view.addSubview(tempestView);
        self.view.addSubview(p1Button);

        // position our views
        positionViews(to: self.view.bounds.size);
        
        // set up our function that gets called on screen updates
        let displayLink : CADisplayLink = CADisplayLink(target: self,selector:#selector(update));
        displayLink.preferredFramesPerSecond = 60;
        displayLink.add(to: RunLoop.current, forMode: RunLoopMode.defaultRunLoopMode);
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    private func positionViews(to size: CGSize) {
        // start by figuring out if we are portrait or landscape
        if (size.width > size.height) {
            // tempest
            self.tempestView.frame = CGRect(x:0,y:0,width:size.height,height:size.height);
            
            // p1button
            p1Button.frame = CGRect(
                x: tempestView.frame.maxX + 10,
                y: 10,
                width: 80,
                height: 80);
        } else {
            // tempest
            self.tempestView.frame = CGRect(x:0,y:0,width:size.width, height:size.width);

            // p1button
            p1Button.frame = CGRect(
                x: 10,
                y: tempestView.frame.maxY + 10,
                width: 80,
                height: 80);
        }
    }
    
    func update()  {
        // ask Tempest for a list of lines to draw
        let vectors : cVectors = cTempestGetVectors(tempest);
        
        // tell the view to draw them
        tempestView.showVectors(vectors:vectors);
        
        // free up our vectors array
        cVectorsDispose(vectors);
    }


    override func viewWillTransition(to size: CGSize, with coordinator: UIViewControllerTransitionCoordinator) {
        positionViews(to: size);
    }
    
    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        // Get the new view controller using segue.destinationViewController.
        // Pass the selected object to the new view controller.
    }
    */

}
