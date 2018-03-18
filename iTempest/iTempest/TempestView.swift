//
//  TempestView.swift
//  iTempest
//
//  Created by Randy Rasmussen on 11/15/17.
//  Copyright © 2017 Randy Rasmussen. All rights reserved.
//

import Foundation
import UIKit
import MetalKit

class TempestView : MTKView {
   private var commandQueue: MTLCommandQueue?
   private var depthStencilState: MTLDepthStencilState?
   private var tempest : cTempest = 0
   private var tempestRenderer : TempestViewRenderer?
   private var spinner : SpinnerView?
   private var spinnerRenderer : SpinnerRenderer?
   private var spinnerHub : SpinnerHub?

   init(tempest : cTempest) {
      // call the super
      super.init(frame: CGRect(), device: MTLCreateSystemDefaultDevice())
      
      // save parameters
      self.tempest = tempest
      
      // View
      clearColor = MTLClearColorMake(0.0, 0.0, 0.0, 1)
      colorPixelFormat = .bgra8Unorm
      depthStencilPixelFormat = .depth32Float
      
      // Command queue
      commandQueue = device!.makeCommandQueue()      
      
      // Depth stencil
      let depthSencilDescriptor = MTLDepthStencilDescriptor()
      depthSencilDescriptor.depthCompareFunction = .less
      depthSencilDescriptor.isDepthWriteEnabled = true
      depthStencilState = device!.makeDepthStencilState(descriptor: depthSencilDescriptor)
      
      // now that we're initialized we can create the renderers
      tempestRenderer = TempestViewRenderer(view: self, tempest: tempest)
      spinnerRenderer = SpinnerRenderer(view:self)
      spinnerHub = SpinnerHub(view:self)

      // add our child views
      spinner = SpinnerView(tempest: tempest, callback:onSpinnerMoved)
      self.addSubview(spinner!)
   }

   required init(coder: NSCoder) {
      super.init(coder: coder)
   }
   
   override func draw(_ dirtyRect: CGRect) {
      // create our parallel render encoder
      let commandBuffer = commandQueue!.makeCommandBuffer()
      let renderPassDescriptor = currentRenderPassDescriptor!
      let parallelRenderEncoder = commandBuffer.makeParallelRenderCommandEncoder(descriptor: renderPassDescriptor)
      
      // create our render encoder for tempest and render it
      var renderEncoder = parallelRenderEncoder.makeRenderCommandEncoder()
      renderEncoder.setFrontFacing(.counterClockwise)
      renderEncoder.setDepthStencilState(depthStencilState)
      tempestRenderer!.render(renderEncoder: renderEncoder)
      renderEncoder.endEncoding()
      
      // create our render encoder for the spinner and render it
      renderEncoder = parallelRenderEncoder.makeRenderCommandEncoder()
      renderEncoder.setFrontFacing(.counterClockwise)
      renderEncoder.setDepthStencilState(depthStencilState)
      spinnerRenderer!.render(renderEncoder: renderEncoder)
      renderEncoder.endEncoding()
      
      // and the hub
      renderEncoder = parallelRenderEncoder.makeRenderCommandEncoder()
      renderEncoder.setFrontFacing(.counterClockwise)
      renderEncoder.setDepthStencilState(depthStencilState)
      spinnerHub!.render(renderEncoder: renderEncoder)
      renderEncoder.endEncoding()
      
      // end encoding on our parallel renderer
      parallelRenderEncoder.endEncoding()
      
      let drawable = currentDrawable!
      commandBuffer.present(drawable)
      commandBuffer.commit()
   }
   
   override func layoutSubviews() {
      // call the super
      super.layoutSubviews()
      
      // although they are not technically subviews, this is a great place to
      // figure out where the metal images should go

      // set the frame of the Tempest graphic
      var width = self.frame.width
      if (width > self.frame.height) {
         width = self.frame.height
      }
      tempestRenderer!.frame = CGRect(x:0.0, y:0.0, width:width, height:width)

      // figuring out if we are portrait or landscape
      if (self.frame.width > self.frame.height) {
         // spinner
         spinnerRenderer!.frame = CGRect(
            x: tempestRenderer!.frame.maxX,
            y: self.frame.height - 80,
            width: self.frame.width - tempestRenderer!.frame.maxX,
            height: 80);
      } else {
         // spinner
         spinnerRenderer!.frame = CGRect(
            x: 0,
            y: self.frame.height - 80,
            width: self.frame.width,
            height: 80);
      }
      
      // and the actual spinner view is the same size... this is just an invisible control
      // that handles the touch input
      spinner!.frame = spinnerRenderer!.frame
      spinnerHub!.frame = spinnerRenderer!.frame
   }
   
   private func onSpinnerMoved(ticks : Int) {
      // convert our encoder tick count to a rotation angle using our magic constant
      let rotation = 1.0 * Float(ticks)
      
      // rotate our graphics
      spinnerRenderer!.rotate(rotationAngle:rotation)
      spinnerHub!.rotate(rotationAngle:rotation)
   }
}

