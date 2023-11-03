/*
 Navicat Premium Data Transfer

 Source Server         : danshen2
 Source Server Type    : MySQL
 Source Server Version : 80100 (8.1.0)
 Source Host           : localhost:3306
 Source Schema         : GantrySystem23

 Target Server Type    : MySQL
 Target Server Version : 80100 (8.1.0)
 File Encoding         : 65001

 Date: 01/11/2023 12:04:52
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for coords_table
-- ----------------------------
DROP TABLE IF EXISTS `coords_table`;
CREATE TABLE `coords_table` (
  `position_id` int NOT NULL AUTO_INCREMENT,
  `X` int NOT NULL,
  `Z` int NOT NULL,
  `Y` int NOT NULL DEFAULT '2000',
  PRIMARY KEY (`position_id`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=17 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- ----------------------------
-- Records of coords_table
-- ----------------------------
BEGIN;
INSERT INTO `coords_table` (`position_id`, `X`, `Z`, `Y`) VALUES (1, 0, 0, 2000);
INSERT INTO `coords_table` (`position_id`, `X`, `Z`, `Y`) VALUES (2, 3200, 0, 2000);
INSERT INTO `coords_table` (`position_id`, `X`, `Z`, `Y`) VALUES (3, 6400, 0, 2000);
INSERT INTO `coords_table` (`position_id`, `X`, `Z`, `Y`) VALUES (4, 9600, 0, 2000);
INSERT INTO `coords_table` (`position_id`, `X`, `Z`, `Y`) VALUES (5, 12800, 0, 2000);
INSERT INTO `coords_table` (`position_id`, `X`, `Z`, `Y`) VALUES (6, 16000, 0, 2000);
INSERT INTO `coords_table` (`position_id`, `X`, `Z`, `Y`) VALUES (7, 0, 22000, 2000);
INSERT INTO `coords_table` (`position_id`, `X`, `Z`, `Y`) VALUES (8, 3200, 22000, 2000);
INSERT INTO `coords_table` (`position_id`, `X`, `Z`, `Y`) VALUES (9, 6400, 22000, 2000);
INSERT INTO `coords_table` (`position_id`, `X`, `Z`, `Y`) VALUES (10, 9600, 22000, 2000);
INSERT INTO `coords_table` (`position_id`, `X`, `Z`, `Y`) VALUES (11, 12800, 22000, 2000);
INSERT INTO `coords_table` (`position_id`, `X`, `Z`, `Y`) VALUES (12, 16000, 22000, 2000);
INSERT INTO `coords_table` (`position_id`, `X`, `Z`, `Y`) VALUES (13, 8000, 44000, 2000);
INSERT INTO `coords_table` (`position_id`, `X`, `Z`, `Y`) VALUES (14, 12000, 44000, 2000);
INSERT INTO `coords_table` (`position_id`, `X`, `Z`, `Y`) VALUES (15, 16000, 44000, 2000);
COMMIT;

-- ----------------------------
-- Table structure for item_colors
-- ----------------------------
DROP TABLE IF EXISTS `item_colors`;
CREATE TABLE `item_colors` (
  `id` int NOT NULL AUTO_INCREMENT,
  `r` int NOT NULL,
  `g` int NOT NULL,
  `b` int NOT NULL,
  `uid` int NOT NULL DEFAULT '0',
  `timein` datetime NOT NULL,
  PRIMARY KEY (`id`,`uid`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=77 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- ----------------------------
-- Records of item_colors
-- ----------------------------
BEGIN;
COMMIT;

-- ----------------------------
-- Table structure for item_info
-- ----------------------------
DROP TABLE IF EXISTS `item_info`;
CREATE TABLE `item_info` (
  `uid` int NOT NULL,
  `eR` int NOT NULL,
  `eG` int NOT NULL,
  `eB` int NOT NULL,
  `height` int DEFAULT NULL,
  `size` int DEFAULT NULL,
  `stackable` tinyint(1) DEFAULT '0',
  `owner` varchar(255) DEFAULT NULL,
  `catagory` varchar(255) DEFAULT NULL,
  `description` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`uid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- ----------------------------
-- Records of item_info
-- ----------------------------
BEGIN;
INSERT INTO `item_info` (`uid`, `eR`, `eG`, `eB`, `height`, `size`, `stackable`, `owner`, `catagory`, `description`) VALUES (11, 103, 65, 83, 30, 1, 1, 'BigTreeTech', 'Electronics', 'TMC2209 Stepper Motor Driver');
INSERT INTO `item_info` (`uid`, `eR`, `eG`, `eB`, `height`, `size`, `stackable`, `owner`, `catagory`, `description`) VALUES (12, 17, 80, 152, 30, 1, 1, 'BigTreeTech', 'Electronics', 'TMC2209 Stepper Motor Driver');
INSERT INTO `item_info` (`uid`, `eR`, `eG`, `eB`, `height`, `size`, `stackable`, `owner`, `catagory`, `description`) VALUES (101, 36, 106, 102, 31, 1, 0, 'MrDanS_Inc', 'Parts', 'Leadscrew Piece V1');
INSERT INTO `item_info` (`uid`, `eR`, `eG`, `eB`, `height`, `size`, `stackable`, `owner`, `catagory`, `description`) VALUES (102, 63, 99, 77, 42, 1, 0, 'MrDanS_Inc', 'Parts', 'Tbar Piece V1');
INSERT INTO `item_info` (`uid`, `eR`, `eG`, `eB`, `height`, `size`, `stackable`, `owner`, `catagory`, `description`) VALUES (108, 20, 75, 155, 25, 2, 0, 'MrDanS_Inc', 'Parts', 'Gripper Piece V1');
INSERT INTO `item_info` (`uid`, `eR`, `eG`, `eB`, `height`, `size`, `stackable`, `owner`, `catagory`, `description`) VALUES (111, 79, 57, 117, 20, 1, 0, 'MrDanS_Inc', 'Electronics', '2x High-Speed Motors');
INSERT INTO `item_info` (`uid`, `eR`, `eG`, `eB`, `height`, `size`, `stackable`, `owner`, `catagory`, `description`) VALUES (201, 27, 69, 154, 60, 1, 0, 'Mobil', 'Resource', '2x Mobil 1 5W-30 Advanced Full Synthetic Motor Oil');
COMMIT;

-- ----------------------------
-- Table structure for item_tags
-- ----------------------------
DROP TABLE IF EXISTS `item_tags`;
CREATE TABLE `item_tags` (
  `id` int NOT NULL,
  `tag` varchar(255) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- ----------------------------
-- Records of item_tags
-- ----------------------------
BEGIN;
COMMIT;

-- ----------------------------
-- Table structure for logs
-- ----------------------------
DROP TABLE IF EXISTS `logs`;
CREATE TABLE `logs` (
  `log_id` int NOT NULL,
  `data` varchar(255) NOT NULL,
  `timestamp` datetime NOT NULL,
  PRIMARY KEY (`log_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- ----------------------------
-- Records of logs
-- ----------------------------
BEGIN;
COMMIT;

-- ----------------------------
-- Table structure for position_table
-- ----------------------------
DROP TABLE IF EXISTS `position_table`;
CREATE TABLE `position_table` (
  `position_id` int NOT NULL AUTO_INCREMENT,
  `id` int DEFAULT NULL,
  PRIMARY KEY (`position_id`)
) ENGINE=InnoDB AUTO_INCREMENT=17 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- ----------------------------
-- Records of position_table
-- ----------------------------
BEGIN;
INSERT INTO `position_table` (`position_id`, `id`) VALUES (1, 74);
INSERT INTO `position_table` (`position_id`, `id`) VALUES (2, 71);
INSERT INTO `position_table` (`position_id`, `id`) VALUES (3, 75);
INSERT INTO `position_table` (`position_id`, `id`) VALUES (4, 73);
INSERT INTO `position_table` (`position_id`, `id`) VALUES (5, 76);
INSERT INTO `position_table` (`position_id`, `id`) VALUES (6, 0);
INSERT INTO `position_table` (`position_id`, `id`) VALUES (7, 0);
INSERT INTO `position_table` (`position_id`, `id`) VALUES (8, 0);
INSERT INTO `position_table` (`position_id`, `id`) VALUES (9, 0);
INSERT INTO `position_table` (`position_id`, `id`) VALUES (10, 0);
INSERT INTO `position_table` (`position_id`, `id`) VALUES (11, 0);
INSERT INTO `position_table` (`position_id`, `id`) VALUES (12, 0);
INSERT INTO `position_table` (`position_id`, `id`) VALUES (13, 0);
INSERT INTO `position_table` (`position_id`, `id`) VALUES (14, 0);
INSERT INTO `position_table` (`position_id`, `id`) VALUES (15, 0);
INSERT INTO `position_table` (`position_id`, `id`) VALUES (16, 0);
COMMIT;

SET FOREIGN_KEY_CHECKS = 1;
