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
        
      // add our subviews
      self.view.addSubview(tempestView!);

      // position our views
      doLayout()
   }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
   override func viewSafeAreaInsetsDidChange() {
      doLayout()
   }
   
   override func viewWillTransition(to size: CGSize, with coordinator: UIViewControllerTransitionCoordinator) {
        //positionViews(to: size);
      coordinator.animate(alongsideTransition: { (context) in
         // During rotation
      }) { [weak self] (context) in
         // After rotation
         self!.doLayout()
      }
   }
    
   private func doLayout() {
      self.tempestView!.frame = self.view.safeAreaLayoutGuide.layoutFrame
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
