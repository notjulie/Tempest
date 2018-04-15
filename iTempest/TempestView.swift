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
   private var player1 : PlayerButtonView?
   private var player2 : PlayerButtonView?
   private var fire : ButtonView?
   private var zap : ButtonView?
   private var screen : SolidRectangleShader?

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
      depthSencilDescriptor.isDepthWriteEnabled = false
      depthStencilState = device!.makeDepthStencilState(descriptor: depthSencilDescriptor)
      
      // now that we're initialized we can create the renderers
      tempestRenderer = TempestViewRenderer(view: self, tempest: tempest)
      spinnerRenderer = SpinnerRenderer(view:self)
      spinnerHub = SpinnerHub(view:self)
      player1 = PlayerButtonView(view:self, tempest:tempest, whichButton:PLAYER1)
      player2 = PlayerButtonView(view:self, tempest:tempest, whichButton:PLAYER2)
      fire = ButtonView(view:self, tempest:tempest, whichButton:FIRE)
      zap = ButtonView(view:self, tempest:tempest, whichButton:ZAP)
      screen = SolidRectangleShader(view:self)

      // add our child views... our views are actually just transparent place holders for
      // receiving user inputs
      spinner = SpinnerView(tempest: tempest, callback:onSpinnerMoved)
      self.addSubview(spinner!)
      self.addSubview(player1!)
      self.addSubview(player2!)
      self.addSubview(fire!)
      self.addSubview(zap!)
   }

   required init(coder: NSCoder) {
      super.init(coder: coder)
   }
   
   /**
    * Called on every frame to perform Metal rendering
    */
   override func draw(_ dirtyRect: CGRect) {
      // create our parallel render encoder
      let commandBuffer = commandQueue!.makeCommandBuffer()
      let renderPassDescriptor = currentRenderPassDescriptor!
      let parallelRenderEncoder = commandBuffer.makeParallelRenderCommandEncoder(descriptor: renderPassDescriptor)

      // set the color of the screen according to whether or not the player buttons
      // should be showing
      if (cTempestIsInAttractMode(tempest) != 0) {
         screen!.setColor(r:0.2, g:0.2, b:0.2, a:0.8)
      } else {
         screen!.setColor(r:0, g:0, b:0, a:0.0)
      }
      
      // render stuff
      render(parallelRenderEncoder:parallelRenderEncoder,renderer:spinnerHub!)
      render(parallelRenderEncoder:parallelRenderEncoder,renderer:spinnerRenderer!)
      render(parallelRenderEncoder:parallelRenderEncoder,renderer:fire!)
      render(parallelRenderEncoder:parallelRenderEncoder,renderer:zap!)
      render(parallelRenderEncoder:parallelRenderEncoder,renderer:tempestRenderer!)

      render(parallelRenderEncoder:parallelRenderEncoder,renderer:screen!)
      render(parallelRenderEncoder:parallelRenderEncoder,renderer:player1!)
      render(parallelRenderEncoder:parallelRenderEncoder,renderer:player2!)

      // end encoding on our parallel renderer
      parallelRenderEncoder.endEncoding()
      
      let drawable = currentDrawable!
      commandBuffer.present(drawable)
      commandBuffer.commit()
   }

   private func render(parallelRenderEncoder:MTLParallelRenderCommandEncoder, renderer:AbstractMetalRenderer) {
      // create our render encoder for tempest and render it
      let renderEncoder = parallelRenderEncoder.makeRenderCommandEncoder()
      renderEncoder.setFrontFacing(.counterClockwise)
      renderEncoder.setDepthStencilState(depthStencilState)
      renderer.render(renderEncoder: renderEncoder)
      renderEncoder.endEncoding()
   }
   
   override func layoutSubviews() {
      // call the super
      super.layoutSubviews()
      
      // set the frame of the Tempest graphic
      var width = self.frame.width
      if (width > self.frame.height) {
         width = self.frame.height
      }
      let tempestFrame = CGRect(x:0.0, y:0.0, width:width, height:width)
      tempestRenderer!.setFrame(frame:tempestFrame)

      // player 1 and player 2 buttons are stacked vertically and centered over the game
      let playerButtonSpacing = CGFloat(100.0)
      let playerButtonWidth = CGFloat(50.0)
      let playerButtonStackHeight = playerButtonWidth*2 + playerButtonSpacing
      let playerButton1Y = tempestFrame.minY + (tempestFrame.height - playerButtonStackHeight) / 2
      let playerButtonX = tempestFrame.minX + (tempestFrame.width - playerButtonWidth) / 2
      player1!.frame = CGRect(
         x:playerButtonX,
         y:playerButton1Y,
         width:playerButtonWidth,
         height:playerButtonWidth
      )
      player2!.frame = CGRect(
         x:playerButtonX,
         y:playerButton1Y + playerButtonWidth + playerButtonSpacing,
         width:playerButtonWidth,
         height:playerButtonWidth
      )
      
      // figuring out if we are portrait or landscape
      var spinnerFrame : CGRect
      if (self.frame.width > self.frame.height) {
         // spinner
         spinnerFrame = CGRect(
            x: tempestFrame.maxX,
            y: self.frame.height - 80,
            width: self.frame.width - tempestFrame.maxX,
            height: 80)
      } else {
         // spinner
         spinnerFrame = CGRect(
            x: 0,
            y: self.frame.height - 80,
            width: self.frame.width,
            height: 80)
      }
      spinnerRenderer!.setFrame(frame:spinnerFrame)
      spinnerHub!.setFrame(frame:spinnerFrame)

      // for the moment just always put fire and zap over the spinner
      fire!.frame = CGRect(
         x: spinnerFrame.minX + 20,
         y: spinnerFrame.minY-80,
         width:80,
         height:80
      )
      zap!.frame = CGRect(
         x: spinnerFrame.maxX-20-80,
         y: spinnerFrame.minY-80,
         width:80,
         height:80
      )
      
      // our screen behind the player 1/ player 2 buttons is the full window
      screen!.setFrame(frame:self.bounds)

      // and the actual spinner view is the same size... this is just an invisible control
      // that handles the touch input
      spinner!.frame = spinnerFrame
   }
   
   private func onSpinnerMoved(ticks : Int) {
      // convert our encoder tick count to a rotation angle using our magic constant
      let rotation = 1.0 * Float(ticks)
      
      // rotate our graphics
      spinnerRenderer!.rotate(rotationAngle:rotation)
      spinnerHub!.rotate(rotationAngle:rotation)
   }
}

