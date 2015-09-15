//
//  PUPaymentService.h
//  PayU-iOS-SDK-Oneclick
//
//  Created by Przemyslaw Stasiak on 24.04.2014.
//  Copyright (c) 2014 PayU S.A. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "PUPaymentRequest.h"
#import "PUPaymentMethodDescription.h"
#import "PUAuthorizationDataSource.h"
#import "PUPaymentMethodViewControllerDelegate.h"


typedef NS_ENUM(NSInteger, PUPaymentRequestStatus) {
    PUPaymentRequestStatusSuccess = 1,
    PUPaymentRequestStatusRetry,
    PUPaymentRequestStatusFailure
};

typedef NS_ENUM(NSInteger, PUPresentationStyle) {
    PUPresentationStyleInsideNavigationController = 1,
    PUPresentationStyleOutsideNavigationController
};

@class PUPaymentService;

@protocol PUPaymentServiceDelegate <NSObject>

/**----------------------------------------------------------------------
 *  @name Presenting View Controller
 * ----------------------------------------------------------------------
 */

/**
 *  This method is invoked when Payment Service detect the need of presenting view controller to the user.
 *  When this call is received view controller **must** be presented on screen to enable user to add or select
 *  payment method.
 *  @warning Given controller **can't be pushed** onto UIViewController stack.
 *
 *  @param viewController Payment methods view controller, authorization view controller etc.
 */
- (void)paymentServiceDidRequestPresentingViewController:(UIViewController *)viewController;

/**
 *  This method is invoked when Payment Service detect the need of presenting payment methods list to the user.
 *  When this call is received Payment Service should be used to retrieve Payment Methods View Controller.
 *  This view controller should be presented on screen.
 *
 *  @param paymentService Payment Service instance.
 *  @deprecated Use paymentServiceDidRequestPresentingPaymentMethodsViewController instead
 */

- (void)paymentServiceDidRequestPresentingPaymentMethodViewController:(PUPaymentService *)paymentService __deprecated_msg("Use paymentServiceDidRequestPresentingPaymentMethodsViewController instead");

/**
 *  This method notifies about changes to payment method that are visible in payment method widget. It is invoked on
 *  various events like: user selects payment method, widgets load previously selected payment method, user deletes 
 *  selected payment method ,etc.
 *
 *  Use this method to enable/disable payment button in merchant application.
 *
 *  @param paymentMethod  Selected payment method or nil, if no method is selected.
 */
@optional
- (void)paymentServiceDidSelectPaymentMethod:(PUPaymentMethodDescription *)paymentMethod;

@end

/**
 *  PUPaymentService class manages all aspects of adding and selecting payment methods and performing payment.
 */
@interface PUPaymentService : NSObject

/**
 *  Authorization data source
 */
@property(nonatomic, weak) id <PUAuthorizationDataSource> dataSource;

/**
 *  PUPaymentService delegate
 */
@property(nonatomic, weak) id <PUPaymentServiceDelegate> delegate;

/**----------------------------------------------------------------------
 *  @name Showing selected payment method
 * ----------------------------------------------------------------------
 */

/**
 *  Method for retrieving payment method widget. This widget is a UIView subclass that shows currently selected payment method.
 *  State and appearance of the widget is controlled internally the only responsibility of the client is to properly show widget on screen.
 *  When user interacts with the widget the Payment Service will inform its delegate that Payment Methods View Controller should be presented on screen.
 *
 *  @param frame Proposed frame for widget.
 *  @warning Widget will reposition its controls to reflect width changes but height is fixed to 50.
 */
- (UIView *)paymentMethodWidgetWithFrame:(CGRect)frame;

/**----------------------------------------------------------------------
 *  @name Submitting payment
 * ----------------------------------------------------------------------
 */

/**
 *  Method for submitting payment.
 *
 *  @param paymentRequest               PUPaymentRequest object containing transaction data.
 *  @param completionHandler            Code block that will be invoked when submitting is finished, or brake at some point with error.
 */
- (void)submitPaymentRequest:(PUPaymentRequest *)paymentRequest
           completionHandler:(void (^)(PUPaymentRequestStatus status, NSError *error))completionHandler;

/**----------------------------------------------------------------------
 *  @name Handling changing of user in merchant application
 * ----------------------------------------------------------------------
 */

/**
 *  Cleans up all data related to current user context.
 *  @note Sample usage scenarios: logging out user for which PUPaymentService instance is currently used,
 *  creating fresh instance of PUPaymentService for user different than last time. 
 * @warning If You are not using widget you must remember to refresh already presented selected payment method of logged out user!
 */
- (void)clearUserContext;

/**----------------------------------------------------------------------
 *  @name Handle response from external application
 * ----------------------------------------------------------------------
 */

/**
 *  This method enables forwarding of callback URL to SDK and is required to properly handle payment authorization with external applications.
 *  Should be called in UIApplicationDelegate in method - (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication
 *  @param callback URL which application was asked to open.
 */
- (BOOL)handleOpenURL:(NSURL *)callback;

/**
 *  Method for submitting payment.
 *
 *  @param paymentRequest               PUPaymentRequest object containing transaction data.
 *  @param completionHandler            Code block that will be invoked when submitting is finished, or brake at some point with error.
 *  @param controllerPresentationBlock  Code block that will be invoked when it is necessary to display some additional view
 * controller to finish payment process, f.e. in order to provide additional authorization infomation (CVV, 3DS) or log in to bank system.
 * This code block provides UIViewController subclass that **must** be shown on the screen in order to properly finish payment process.
 *  @warning controllerPresentationBlock code block may be called several times.
 */
- (void)submitPaymentRequest:(PUPaymentRequest *)paymentRequest
           completionHandler:(void (^)(PUPaymentRequestStatus status, NSError *error))completionHandler
 controllerPresentationBlock:(void (^)(UIViewController *viewController, PUPresentationStyle presentationStyle))controllerPresentationBlock __deprecated_msg("Use - submitPaymentRequest: completionHandler: instead");

/**----------------------------------------------------------------------
 *  @name Retrieving selected payment method
 * ----------------------------------------------------------------------
 */

/**
 *  Method for retrieving selected payment method.
 *
 *  @param completionHandler Code block that will be invoked when retrieving is finished
 *  @deprecated Use PaymentMethodWidget to present selected payment method instead
 */
- (void)retrieveSelectedPaymentMethodWithCompletionHandler:(void (^)(PUPaymentMethodDescription *paymentMethod, NSError *error))completionHandler __deprecated_msg("Use PaymentMethodWidget to present selected payment method instead");

/**----------------------------------------------------------------------
 *  @name Providing payment methods list UIViewController subclass
 * ----------------------------------------------------------------------
 */

/**
 *  Method for providing UIViewController subclass containing payment methods list. Provided instance should be presented on the screen.
 *  @warning Given controller **can't** be pushed onto UIViewController stack.
 *
 *  @param delegate Delegate that will be informed about payment method selection.
 *  @return UIViewController subclass for showing payment methods list.
 *  @deprecated Use widget and implement paymentServiceDidRequestPresentingPaymentMethodsViewController instead.
 */
- (UIViewController *)paymentMethodViewControllerWithDelegate:(id <PUPaymentMethodViewControllerDelegate>)delegate __deprecated_msg("Use widget and implement - paymentServiceDidRequestPresentingPaymentMethodsViewController instead");

@end
