-- retrieving
DBCC checkident ('Employees')

-- repairing the Identity Seed 
DBCC checkident ('Employees', reseed)

-- changing the Identity Seed 
DBCC checkident ('Employees', reseed, 1000)