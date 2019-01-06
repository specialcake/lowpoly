//
//  AppDelegate.swift
//  Polyworld
//
//  Created by 沈心逸 on 2018/12/19.
//  Copyright © 2018年 Sxy. All rights reserved.
//

import UIKit
import MessageUI
import SafariServices

@UIApplicationMain
class AppDelegate: UIResponder, UIApplicationDelegate, MFMailComposeViewControllerDelegate {

    var window: UIWindow?
    
    var blockRotation: UIInterfaceOrientationMask = .landscape {
        didSet{
            if blockRotation.contains(.portrait){
                //强制设置成竖屏
                UIDevice.current.setValue(UIInterfaceOrientation.portrait.rawValue, forKey: "orientation")
            }else{
                //强制设置成横屏
                UIDevice.current.setValue(UIInterfaceOrientation.landscapeLeft.rawValue, forKey: "orientation")
                
            }
        }
    }

    func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplication.LaunchOptionsKey: Any]?) -> Bool {
        let playIcon = UIApplicationShortcutIcon.init(type: .play)
        let playItem = UIApplicationShortcutItem.init(type: "play", localizedTitle: "进入Polyworld", localizedSubtitle: nil, icon: playIcon, userInfo: nil)
        
        let ARIcon = UIApplicationShortcutIcon.init(type: .capturePhoto)
        let ARItem = UIApplicationShortcutItem.init(type: "AR", localizedTitle: "体验AR", localizedSubtitle: nil, icon: ARIcon, userInfo: nil)
        
//        let loveIcon = UIApplicationShortcutIcon.init(type: .love)
//        let loveItem = UIApplicationShortcutItem.init(type: "mail", localizedTitle: "点赞", localizedSubtitle: nil, icon: loveIcon, userInfo: nil)
        
        let mailIcon = UIApplicationShortcutIcon.init(type: .mail)
        let mailItem = UIApplicationShortcutItem.init(type: "mail", localizedTitle: "与开发者联系", localizedSubtitle: nil, icon: mailIcon, userInfo: nil)
        
        let shareIcon = UIApplicationShortcutIcon.init(type: .share)
        let shareItem = UIApplicationShortcutItem.init(type: "share", localizedTitle: "分享给好友", localizedSubtitle: nil, icon: shareIcon, userInfo: nil)
        
        UIApplication.shared.shortcutItems = [playItem, ARItem, mailItem, shareItem]
        return true
    }

    func applicationWillResignActive(_ application: UIApplication) {
        // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
        // Use this method to pause ongoing tasks, disable timers, and invalidate graphics rendering callbacks. Games should use this method to pause the game.
    }

    func applicationDidEnterBackground(_ application: UIApplication) {
        // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
        // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
    }

    func applicationWillEnterForeground(_ application: UIApplication) {
        // Called as part of the transition from the background to the active state; here you can undo many of the changes made on entering the background.
    }

    func applicationDidBecomeActive(_ application: UIApplication) {
        // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
    }

    func applicationWillTerminate(_ application: UIApplication) {
        // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
    }
    
    func application(_ application: UIApplication, performActionFor shortcutItem: UIApplicationShortcutItem, completionHandler: (Bool) -> Void) {
        switch shortcutItem.type {
        case "play":
            print("play game")
        case "AR":
            window?.rootViewController?.performSegue(withIdentifier: "ARmode", sender: nil)
        case "mail":
            guard MFMailComposeViewController.canSendMail() else {
                print("Can not send mail")
                return
            }
            
            let mailComposer = MFMailComposeViewController()
            mailComposer.mailComposeDelegate = self
            mailComposer.setToRecipients(["shenxinyi@zju.edu.cn"])
            mailComposer.setSubject("About Polyworld")
            mailComposer.setMessageBody("Hello, this is an email from me.", isHTML: false)
            blockRotation = .portrait
            window?.rootViewController?.present(mailComposer, animated: false, completion: nil)
        case "share":
            guard let window = UIApplication.shared.keyWindow else { return }
            
            // 用下面这行而不是UIGraphicsBeginImageContext()，因为前者支持Retina
            UIGraphicsBeginImageContextWithOptions(window.bounds.size, false, 0.0)
            
            window.layer.render(in: UIGraphicsGetCurrentContext()!)
            
            let image = UIGraphicsGetImageFromCurrentImageContext()
            
            UIGraphicsEndImageContext()
            
            let activityController = UIActivityViewController(activityItems: [image], applicationActivities: nil)
            activityController.popoverPresentationController?.sourceView = window.rootViewController?.view
            window.rootViewController?.present(activityController, animated: true, completion: nil)
        default:
            break
        }
    }
    
    func mailComposeController(_ controller: MFMailComposeViewController, didFinishWith result: MFMailComposeResult, error: Error?) {
        blockRotation = .landscape
        window?.rootViewController?.dismiss(animated: false, completion: nil)
    }
    
}

extension AppDelegate{
    
    func application(_ application: UIApplication, supportedInterfaceOrientationsFor window: UIWindow?) -> UIInterfaceOrientationMask {
        
        return blockRotation
    }
}
