
UPDATE THIS - NOT CORRECT OR CURRENT !!!!!
UPDATE THIS - NOT CORRECT OR CURRENT !!!!!
UPDATE THIS - NOT CORRECT OR CURRENT !!!!!
UPDATE THIS - NOT CORRECT OR CURRENT !!!!!
UPDATE THIS - NOT CORRECT OR CURRENT !!!!!
UPDATE THIS - NOT CORRECT OR CURRENT !!!!!


--
-- Table structure for table `work_employees`
--

DROP TABLE IF EXISTS `work_employees`;
CREATE TABLE IF NOT EXISTS `work_employees` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(25) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=9 DEFAULT CHARSET=utf8mb4 COMMENT='managed by workadmin on dcsatl site';

--
-- Dumping data for table `work_employees`
--

INSERT INTO `work_employees` (`id`, `name`, `debug`) VALUES
(1, 'Miguel', '0'),
(2, 'Max', '0'),
(3, 'Edelmira', '0'),
(4, 'Margarito', '0'),
(5, 'Cesar', '0');

-- --------------------------------------------------------

--
-- Table structure for table `work_jobs`
--

DROP TABLE IF EXISTS `work_jobs`;
CREATE TABLE IF NOT EXISTS `work_jobs` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `jobname` varchar(150) DEFAULT NULL,
  `address_line1` varchar(100) DEFAULT NULL,
  `address_line2` varchar(100) DEFAULT NULL,
  `city` varchar(50) DEFAULT NULL,
  `state` varchar(2) DEFAULT NULL,
  `zip` varchar(12) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `jobname` (`jobname`)
) ENGINE=MyISAM AUTO_INCREMENT=9 DEFAULT CHARSET=utf8mb4 COMMENT='managed by workadmin on dcsatl site';

--
-- Dumping data for table `work_jobs`
--

INSERT INTO `work_jobs` (`id`, `jobname`, `address_line1`, `address_line2`, `city`, `state`, `zip`, `debug`) VALUES
(1, 'Bruce', NULL, NULL, NULL, NULL, NULL, '0'),
(2, 'Parkaire', NULL, NULL, NULL, NULL, NULL, '0'),
(3, 'West Village', NULL, NULL, NULL, NULL, NULL, '0'),
(4, 'Warehouse Cubed', NULL, NULL, NULL, NULL, NULL, '0'),
(5, 'Cottages Of Avalon', NULL, NULL, NULL, NULL, NULL, '0'),
(6, 'Wade Green 75', NULL, NULL, NULL, NULL, NULL, '0');

-- --------------------------------------------------------

--
-- Table structure for table `work_payments`
--

DROP TABLE IF EXISTS `work_payments`;
CREATE TABLE IF NOT EXISTS `work_payments` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `jobs_id` int(11) NOT NULL,
  `employees_id` int(11) NOT NULL,
  `date_paid` date DEFAULT NULL,
  `payment` float(15,2) NOT NULL DEFAULT 0.00,
  `materials` float(15,2) NOT NULL DEFAULT 0.00,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=109 DEFAULT CHARSET=utf8mb4 COMMENT='managed by workadmin on dcsatl site';

--
-- Dumping data for table `work_payments`
--

INSERT INTO `work_payments` (`id`, `jobs_id`, `employees_id`, `date_paid`, `payment`, `materials`, `debug`) VALUES
(108, 5, 4, '2020-07-08', 125.00, 55.94, NULL),
(107, 4, 5, '2020-07-08', 12.98, 89.65, NULL),
(106, 1, 1, '2020-07-06', 12.22, 31.13, NULL),
(3, 2, 3, '2020-07-08', 22.33, 0.00, NULL),
(2, 2, 2, '2020-07-08', 99.88, 88.77, NULL),
(1, 1, 1, '2020-07-08', 12.34, 33.00, NULL);

-- --------------------------------------------------------

--
-- Table structure for table `work_users`
--

DROP TABLE IF EXISTS `work_users`;
CREATE TABLE IF NOT EXISTS `work_users` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(45) NOT NULL,
  `password` varchar(20) NOT NULL DEFAULT 'password321',
  `date_created` datetime NOT NULL DEFAULT current_timestamp(),
  `last_login` datetime DEFAULT NULL,
  `is_active` tinyint(1) NOT NULL COMMENT '0=not active; 1=active',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=7 DEFAULT CHARSET=utf8mb4 COMMENT='managed by workadmin on dcsatl site';

--
-- Dumping data for table `work_users`
--

INSERT INTO `work_users` (`id`, `username`, `password`, `date_created`, `last_login`, `is_active`) VALUES
(1, 'tim', 'pw', '2020-06-27 21:06:20', '2020-06-27 21:06:20', 1, 0),
(2, 'jcdeen', 'pw', '2020-06-27 21:06:20', '2020-06-27 21:06:20', 1, 0),
(3, 'Ann', 'pw', '2020-06-27 21:06:20', '2020-06-27 21:06:20', 0, 0),
(4, 'Cesar', 'pw', '2020-06-27 21:06:20', '2020-06-27 21:06:20', 1, 0),
(5, 'Edwardo', 'pw', '2020-06-27 21:06:20', '2020-06-27 21:06:20', 0, 0),
(6, 'Michael', 'pw', '2020-06-27 21:06:20', '2020-06-27 21:06:20', 0, 0);
COMMIT;
