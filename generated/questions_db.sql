/* This file was generated by ODB, object-relational mapping (ORM)
 * compiler for C++.
 */

DROP TABLE IF EXISTS `questions`;

CREATE TABLE `questions` (
  `id` INT UNSIGNED NOT NULL PRIMARY KEY AUTO_INCREMENT,
  `text` TEXT NOT NULL,
  `answer` TEXT NOT NULL,
  `can_view_answer` TINYINT(1) NOT NULL,
  `selected` TINYINT(1) NOT NULL)
 ENGINE=InnoDB;

