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
   private let tempestLayer : TempestLayer = TempestLayer();

   public func showVectors(vectors:cVectors)
   {
      if (!initialized)
      {
         self.backgroundColor = UIColor.black;
         self.layer.addSublayer(tempestLayer);
         initialized = true;
      }

      tempestLayer.frame = self.bounds;
      tempestLayer.showVectors(vectors:vectors);
    }
}

