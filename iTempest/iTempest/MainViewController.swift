//
//  MainViewController.swift
//  iTempest
//
//  Created by Randy Rasmussen on 11/14/17.
//  Copyright © 2017 Randy Rasmussen. All rights reserved.
//

import UIKit
import AVFoundation

class MainViewController: UIViewController {
   private var tempestView : TempestView?
   private var tempest : cTempest = 0
   private var p1Button : ButtonView?
   private var fireButton : ButtonView?
   private var zapButton : ButtonView?

   override func viewDidLoad() {
      // call the super
      super.viewDidLoad();
      
      // initialize the audio session before instantiating Tempest
      do {
         try AVAudioSession.sharedInstance().setPreferredIOBufferDuration(0.01);
      } catch {
      }

      // create Tempest
      tempest = cTempestCreate()
      
      // create TempestView
      tempestView = TempestView(tempest:tempest)
      
      // set our background
      self.view.layer.backgroundColor = UIColor.brown.cgColor;
        
      // initialize our other subviews
      fireButton = ButtonView(tempest:tempest, _whichButton:FIRE, color:UIColor.blue);
      p1Button = ButtonView(tempest:tempest, _whichButton:PLAYER1, color:UIColor.red);
      zapButton = ButtonView(tempest:tempest, _whichButton:ZAP, color:UIColor.green);

      // add our subviews
      self.view.addSubview(tempestView!);
      self.view.addSubview(p1Button!);
      self.view.addSubview(fireButton!);
      self.view.addSubview(zapButton!);

      // position our views
      positionViews(to: self.view.bounds.size);
   }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
   private func positionViews(to size: CGSize) {
      // tempest view gets the full frame
      self.tempestView!.frame = CGRect(x:0,y:0,width:size.width,height:size.height);

      // start by figuring out if we are portrait or landscape
      if (size.width > size.height) {
         // p1button
         p1Button!.frame = CGRect(
             x: tempestView!.frame.maxX + 10,
             y: 10,
             width: 80,
             height: 80);

         // zapbutton
         zapButton!.frame = CGRect(
             x: tempestView!.frame.maxX + 110,
             y: 10,
             width: 80,
             height: 80);

         // firebutton
         fireButton!.frame = CGRect(
             x: tempestView!.frame.maxX + 200,
             y: 10,
             width: 80,
             height: 80);
      } else {
         // p1button
         p1Button!.frame = CGRect(
             x: 10,
             y: tempestView!.frame.maxY + 10,
             width: 80,
             height: 80);

         // zapbutton
         zapButton!.frame = CGRect(
             x: 110,
             y: tempestView!.frame.maxY + 10,
             width: 80,
             height: 80);

         // firebutton
         fireButton!.frame = CGRect(
             x: 200,
             y: tempestView!.frame.maxY + 10,
             width: 80,
             height: 80);
      }
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
