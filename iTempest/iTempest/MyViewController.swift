//
//  MyViewController.swift
//  iTempest
//
//  Created by Randy Rasmussen on 11/14/17.
//  Copyright © 2017 Randy Rasmussen. All rights reserved.
//

import UIKit

class MyViewController: UIViewController {
    var tempestView : TempestView = TempestView();
    var tempest : cTempest = cTempestCreate();

    override func viewDidLoad() {
        // call the super
        super.viewDidLoad();
        
        // set our background
        self.view.layer.backgroundColor = UIColor.brown.cgColor;
        
        // initialize the TempestView
        self.tempestView.frame = CGRect(x:0,y:0,width:200,height:200);
        self.tempestView.backgroundColor = UIColor.red;
        self.view.addSubview(tempestView);
        
        // set up our function that gets called on screen updates
        let displayLink : CADisplayLink = CADisplayLink(target: self,selector:#selector(update));
        displayLink.preferredFramesPerSecond = 60;
        displayLink.add(to: RunLoop.current, forMode: RunLoopMode.defaultRunLoopMode);
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    func update()  {
        // ask Tempest for a list of lines to draw
        let vectors : cVectors = cTempestGetVectors(tempest);
        
        // tell the view to draw them
        tempestView.showVectors(vectors);
        
        // free up our vectors array
        cVectorsDispose(vectors);
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
