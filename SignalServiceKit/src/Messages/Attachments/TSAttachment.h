//
// Copyright 2017 Signal Messenger, LLC
// SPDX-License-Identifier: AGPL-3.0-only
//

#import <SignalServiceKit/BaseModel.h>

NS_ASSUME_NONNULL_BEGIN

@class SDSAnyReadTransaction;
@class TSAttachmentPointer;
@class TSMessage;

typedef NS_ENUM(NSUInteger, TSAttachmentType) {
    TSAttachmentTypeDefault = 0,
    TSAttachmentTypeVoiceMessage = 1,
    TSAttachmentTypeBorderless = 2,
    TSAttachmentTypeGIF = 3,
};

typedef NS_CLOSED_ENUM(NSUInteger, TSAnimatedMimeType) {
    /// The MIME type is for something that doesn't support animated images.
    TSAnimatedMimeTypeNotAnimated = 0,
    /// The MIME type is for images that may or may not be animated, e.g. "png".
    /// Check the contents to determine if its actually animated.
    TSAnimatedMimeTypeMaybeAnimated = 1,
    /// The MIME type is exclusive to animated images
    TSAnimatedMimeTypeAnimated = 2,
};

@interface TSAttachment : BaseModel

// TSAttachment is a base class for TSAttachmentPointer (a yet-to-be-downloaded
// incoming attachment) and TSAttachmentStream (an outgoing or already-downloaded
// incoming attachment).
@property (atomic, readwrite) UInt64 serverId;
@property (atomic) NSString *cdnKey;
@property (atomic) UInt32 cdnNumber;
@property (atomic, readwrite, nullable) NSData *encryptionKey;
@property (nonatomic, readonly) NSString *contentType;
@property (nonatomic) TSAttachmentType attachmentType;

// Though now required, may incorrectly be 0 on legacy attachments.
@property (nonatomic, readonly) UInt32 byteCount;

// Represents the "source" filename sent or received in the protos,
// not the filename on disk.
@property (nonatomic, readonly, nullable) NSString *sourceFilename;

@property (nonatomic, readonly, nullable) NSString *blurHash;

// This property will be non-zero if set.
@property (nonatomic) UInt64 uploadTimestamp;

#pragma mark - Media Album

@property (nonatomic, readonly, nullable) NSString *caption;
@property (nonatomic, readonly, nullable) NSString *albumMessageId;
@property (nonatomic, readonly) NSString *emoji;

- (nullable TSMessage *)fetchAlbumMessageWithTransaction:(SDSAnyReadTransaction *)transaction
    NS_SWIFT_NAME(fetchAlbumMessage(transaction:));

// `migrateAlbumMessageId` is only used in the migration to the new multi-attachment message scheme,
// and shouldn't be used as a general purpose setter. Instead, `albumMessageId` should be passed as
// an initializer param.
- (void)migrateAlbumMessageId:(NSString *)albumMesssageId;

#pragma mark -

+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)init NS_UNAVAILABLE;
- (nullable instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithUniqueId:(NSString *)uniqueId NS_UNAVAILABLE;
- (instancetype)initWithGrdbId:(int64_t)grdbId uniqueId:(NSString *)uniqueId NS_UNAVAILABLE;

// This constructor is used for new instances of TSAttachmentPointer,
// i.e. undownloaded incoming attachments.
- (instancetype)initWithServerId:(UInt64)serverId
                          cdnKey:(NSString *)cdnKey
                       cdnNumber:(UInt32)cdnNumber
                   encryptionKey:(NSData *)encryptionKey
                       byteCount:(UInt32)byteCount
                     contentType:(NSString *)contentType
                  sourceFilename:(nullable NSString *)sourceFilename
                         caption:(nullable NSString *)caption
                  albumMessageId:(nullable NSString *)albumMessageId
                        blurHash:(nullable NSString *)blurHash
                 uploadTimestamp:(unsigned long long)uploadTimestamp
                   videoDuration:(nullable NSNumber *)videoDuration NS_DESIGNATED_INITIALIZER;

// This constructor is used for new instances of TSAttachmentPointer,
// i.e. undownloaded restoring attachments.
- (instancetype)initForRestoreWithUniqueId:(NSString *)uniqueId
                               contentType:(NSString *)contentType
                            sourceFilename:(nullable NSString *)sourceFilename
                                   caption:(nullable NSString *)caption
                            albumMessageId:(nullable NSString *)albumMessageId NS_DESIGNATED_INITIALIZER;

// This constructor is used for new instances of TSAttachmentStream
// that represent new, un-uploaded outgoing attachments.
- (instancetype)initAttachmentWithContentType:(NSString *)contentType
                                    byteCount:(UInt32)byteCount
                               sourceFilename:(nullable NSString *)sourceFilename
                                      caption:(nullable NSString *)caption
                               albumMessageId:(nullable NSString *)albumMessageId NS_DESIGNATED_INITIALIZER;

// This constructor is used for new instances of TSAttachmentStream
// that represent downloaded incoming attachments.
- (instancetype)initWithPointer:(TSAttachmentPointer *)pointer
                    transaction:(SDSAnyReadTransaction *)transaction NS_DESIGNATED_INITIALIZER;

// --- CODE GENERATION MARKER

// This snippet is generated by /Scripts/sds_codegen/sds_generate.py. Do not manually edit it, instead run `sds_codegen.sh`.

// clang-format off

- (instancetype)initWithGrdbId:(int64_t)grdbId
                      uniqueId:(NSString *)uniqueId
                  albumMessageId:(nullable NSString *)albumMessageId
         attachmentSchemaVersion:(NSUInteger)attachmentSchemaVersion
                  attachmentType:(TSAttachmentType)attachmentType
                        blurHash:(nullable NSString *)blurHash
                       byteCount:(unsigned int)byteCount
                         caption:(nullable NSString *)caption
                          cdnKey:(NSString *)cdnKey
                       cdnNumber:(unsigned int)cdnNumber
                     contentType:(NSString *)contentType
                   encryptionKey:(nullable NSData *)encryptionKey
                        serverId:(unsigned long long)serverId
                  sourceFilename:(nullable NSString *)sourceFilename
                 uploadTimestamp:(unsigned long long)uploadTimestamp
                   videoDuration:(nullable NSNumber *)videoDuration
NS_DESIGNATED_INITIALIZER NS_SWIFT_NAME(init(grdbId:uniqueId:albumMessageId:attachmentSchemaVersion:attachmentType:blurHash:byteCount:caption:cdnKey:cdnNumber:contentType:encryptionKey:serverId:sourceFilename:uploadTimestamp:videoDuration:));

// clang-format on

// --- CODE GENERATION MARKER

- (void)upgradeAttachmentSchemaVersionIfNecessary;
- (void)upgradeFromAttachmentSchemaVersion:(NSUInteger)attachmentSchemaVersion;

@property (nonatomic, readonly) TSAnimatedMimeType isAnimatedMimeType;
@property (nonatomic, readonly) BOOL isImageMimeType;
@property (nonatomic, readonly) BOOL isWebpImageMimeType;
@property (nonatomic, readonly) BOOL isVideoMimeType;
@property (nonatomic, readonly) BOOL isAudioMimeType;
// TODO: This is a property of the containing message.
@property (nonatomic, readonly) BOOL isVoiceMessage;
// TODO: This is a property of the containing message.
@property (nonatomic, readonly) BOOL isBorderless;
// TODO: This is a property of the containing message AND the attachment.
@property (nonatomic, readonly) BOOL isLoopingVideo;
@property (nonatomic, readonly) BOOL isVisualMediaMimeType;
@property (nonatomic, readonly) BOOL isOversizeTextMimeType;

// nil: no value cached
// NaN: not a video, broken video, or duration otherwise impossible to ascertain.
// Nonnegative number: Duration in seconds
@property (nullable, nonatomic, readonly) NSNumber *videoDuration;

+ (NSString *)emojiForMimeType:(NSString *)contentType;

// This should only ever be used before the attachment is saved,
// after that point the content type will be already set.
- (void)setDefaultContentType:(NSString *)contentType;

// This method should only be called on instances which have
// not yet been inserted into the database.
- (void)replaceUnsavedContentType:(NSString *)contentType NS_SWIFT_NAME(replaceUnsavedContentType(_:));

#pragma mark - Update With...

- (void)updateWithBlurHash:(NSString *)blurHash transaction:(SDSAnyWriteTransaction *)transaction;
- (void)updateWithVideoDuration:(nullable NSNumber *)videoDuration transaction:(SDSAnyWriteTransaction *)transaction;

@end

NS_ASSUME_NONNULL_END
