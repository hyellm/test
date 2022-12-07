/* -- MyVocaList.cpp -- */

#include "MyVocaList.h"

MyVoca myToeicVocaList[] =
{
	/* 1 ~ 10 */
	MyVoca("mean", NOUN, { "average", "norm", "median", "middle", "midpoint", "(ant) extremity" }, { "the mean error", "the golden mean", "the arithmetical mean", "the geometric mean" }),
	MyVoca("mean", ADJ, { "nasty", "poor", "middle", "miserly", "paltry" }, { "a man of mean intelligence", "a mean appearance" }),
	MyVoca("mean", VERB, { "require", "denote", "intend" }, { "What do you mean by \"perfect\" \?" }),
	MyVoca("offer", NOUN, { "proposal" }, { "He accepted out offer to write the business plan." }),
	MyVoca("offer", VERB, { "to propose" }, { "She must offer her banker new statistics in order to satisfy the bank's requirement for the loan." }),
	MyVoca("compromise", NOUN, { "give-and-take", "bargaining", "accommodation" }, { "The couple made a compromise and ordered food to take out." }),
	MyVoca("compromise", VERB, { "settle", "conciliate", "find a middle ground" }, { "He does not like sweet dishes so I compromised by adding just a small amount of sugar." }),
	MyVoca("delegate", NOUN, { "representative", "agent", "substitute" }, { "" }),
	MyVoca("delegate", VERB, { "authorize", "appoint", "designate" }, { "" }),
	MyVoca("foster", VERB, { "nurture", "raise", "promote", "advance" }, { "" }),

	/* 11 ~ 20 */
	MyVoca("foster", ADJ, { "substitute", "adoptive", "stand-in" }, { "" }),
	MyVoca("imperative", ADJ, { "authoritative", "vital" }, { "" }),
	MyVoca("imperative", NOUN, { "necessity", "essential", "requirement" }, { "" }),
	MyVoca("ensure", VERB, { "assure" }, { "The receptionist called to ensure the message was delivered." }),
	MyVoca("shipment", NOUN, { "freight", "cargo" }, { "The shipment was sent to the wrong port." }),
	MyVoca("adequately", ADV, { "properly", "appropriately" }, {"The workers ensure that glassware is adequately wrapped."}),
	MyVoca("efficient", ADJ, {"effective"}, {"Seal-wrap is an efficient means of packaging."}),
	MyVoca("attach", VERB, {"affix"}, {"Carefully attach the address label to the package."}),
	MyVoca("handle", VERB, {"treat", "manage"}, {"The hazardous substances must be handled with care."}),
	MyVoca("impose", VERB, {"levy"}, {"The government plans to impose taxes on imported steel."}),

	/* 21 ~ 30 */
	MyVoca("detach", VERB, {"separate"}, {"Please detach and send in the completed form."}),
	MyVoca("incorrect", ADJ, {"inaccurate"}, {"Incorrect mailing information will slow the order process."}),
	MyVoca("complimentary", ADJ, {"free"}, {"Guests are given a complimentary light breakfast."}),
	MyVoca("flavor", NOUN, {"savor"}, {"The shop sells ice cream in a variety of flavors."}),
	MyVoca("reservation", NOUN, {"booking"}, {"Once you receive confirmation by e-mail, the reservation has been made."}),
	MyVoca("confirm", VERB, {"verify"}, {"Please confirm your seating reservation prior to arrival."}),
	MyVoca("rate", NOUN, {"fee"}, {"The inn offers fine rooms at affordable rates."}),
	MyVoca("retain", VERB, {"maintain", "keep"}, {"The cafeteria retains customers by offering inexpensive, flavorful food."}),
	MyVoca("atmosphere", NOUN, {"mood"}, {"The hotel provides a comfortable atmosphere."}),
	MyVoca("extensive", ADJ, {"comprehansive", "diverse"}, {"The restaurant offers an extensive range of Chinese dishes."}),

	/* 31 ~ 40 */
	MyVoca("spot", VERB, {"notice"}, {"When preparing to pay for the meal, Marie spotted a mistake on the bill."}),
	MyVoca("spot", NOUN, {"notice"}, {"Midland Park is a good spot for a company picnic."}),
	MyVoca("decline", NOUN, {"decrease", "reduction", "reject"}, {"A sharp decline in the number of buyers has lowered this year's profits."}),
	MyVoca("decline", VERB, {"decrease", "reduction", "reject"}, {"The investor declined our invitation to lunch."}),
	MyVoca("revenue", NOUN, {"income", "earnings"}, {"The company's revenue was boosted by higher album sales."}),
	MyVoca("projection", NOUN, {"estimate"}, {"This month's income projection are higher than last month's were."}),
	MyVoca("anticipate", VERB, {"expect"}, {"We anticipate a 40 percent increase in sales next year."}),
	MyVoca("shift", VERB, {""}, {"The firm shifted some capital into its newest investment project."}),
	MyVoca("shift", NOUN, {""}, {"The night shift is from midnight to 8 A.M."}),
	MyVoca("exceed", VERB, {"surpass"}, {"The new restaurant's profits exceeded initial projections."}),

	/* 41 ~ 50 */
	MyVoca("figure", NOUN, {"number"}, {"Last quarter's sales figures need to be sent to the main office."}),
	MyVoca("achieve", VERB, {"reach"}, {"The corporation achieved its sales goals for the year."}),
	MyVoca("share", VERB, {"discuss"}, {"The CEO shared some excess profits with employees through bonuses."}),
	MyVoca("share", NOUN, {"discuss"}, {"Each partner will receive an equal share of profits from the sale of the company."}),
	MyVoca("reduce", VERB, {"diminish", "decrease"}, {"Management reduced the travel budget in an effort to cut costs."}),
	MyVoca("curtail", VERB, {"reduce"}, {"The manager made an effort to curtail office expenses."}),
	MyVoca("deficit", NOUN, {"shortfall"}, {"Reserve funds will be used to make up for the deficit."}),
	MyVoca("recently", ADV, {"lately"}, {"Bookkeeping costs have recently risen considerably."}),
	MyVoca("substantially", ADV, {"significantly", "considerably"}, {"The marketing team was substantially expanded to help boost sales."}),
	MyVoca("allocate", VERB, {"assign"}, {"Funds were allocated for the charity benefit."}),

	/* 51 ~ 60 */
	MyVoca("amend", VERB, {"revise", "modify"}, {""}),
	MyVoca("expenditure", NOUN, {"expense"}, {"This month's sales outweigh expenditures."}),
	MyVoca("worth", ADJ, {""}, {"It is worth the cost to upgrade our machinery."}),
	MyVoca("worth", NOUN, {""}, {"$200,000 worth of inventory was added last month alone."}),
	MyVoca("liable", ADJ, {"likely"}, {"The guarantor is liable for any unpaid debts."}),
	MyVoca("foresee", VERB, {"predict"}, {"Food companies try to foresee future trends in agriculture."}),
	MyVoca("asset", NOUN, {"estate", "property"}, {"Wilcox Inc. regards its employees as its most valuable assets."}),
	MyVoca("dedicated", ADJ, {"devoted", "committed"}, {"The new director is dedicated to improving the firm's public image."}),
	MyVoca("merge", VERB, {"amalgamate"}, {"The private firm merged with a corporate giant."}),
	MyVoca("imply", VERB, {"suggest"}, {""}),

	/* 61 ~ 70 */
	MyVoca("informed", ADJ, {""}, {"Seeking legal advice will help you make an informed decision."}),
	MyVoca("run", VERB, {"operate", "manage"}, {"The organization is run by retired executives."}),
	MyVoca("edge", NOUN, {"border"}, {"Property prices are far cheaper on the edge of town than in the center."}),
	MyVoca("consensus", VERB, {"agreement"}, {"The general consensus seems to be that selling is the best option."}),
	MyVoca("defer", VERB, {"postpone", "delay"}, {"The registration deadline has been deferred for one week."}),
	MyVoca("report", VERB, {"come"}, {"The head researcher reported his findings to the department leaders."}),
	MyVoca("report", NOUN, {"come"}, {"Frank presented a report on his consumer study findings."}),
	MyVoca("easy", ADJ, {"smooth"}, {"The decision to sell the shopping mall was not easy to make."}),
	MyVoca("record", NOUN, {"register"}, {"Managers reviewed the account record before making a final decision."}),
	MyVoca("record", VERB, {"register"}, {"The secretary recorded everything that was said at the gathering."}),

	/* 71 ~ 80 */
	MyVoca("discuss", VERB, {"share"}, {""}),
	MyVoca("press", NOUN, {"media"}, {"The press covered the merger talks closely."}),
	MyVoca("press", VERB, {"media"}, {"The lecturer pressed the button to lower the screen."}),
	MyVoca("purpose", NOUN, {"aim"}, {""}),
	MyVoca("enroll", VERB, {"register", "sign up"}, {"Employees must enroll in at least one program."}),
	MyVoca("leave", NOUN, {"absence"}, {"Employees can take up to 10 days annually for emergency leave."}),
	MyVoca("leave", VERB, {"absence"}, {"The sales manager left for Singapore to conduct the orientation for sales staff."}),
	MyVoca("include", VERB, {"contain"}, {"The workshop curriculum includes special digital media classes."}),
	MyVoca("regard", VERB, {"view"}, {"Workers regard prompt salary payment as a basic right."}),
	MyVoca("regard", NOUN, {"view"}, {"The company showed little regard for employee welfare."}),

	/* 81 ~ 90 */
	MyVoca("tentative", ADJ, {"temporary", "indefinite"}, {"The plan to increase paternity leave is still tentative."}),
	MyVoca("commence", VERB, {"begin"}, {"The new shifts will commence next week."}),
	MyVoca("objective", NOUN, {"purpose"}, {"The learning objectives of the program are outlined in the brochure."}),
	MyVoca("labor", NOUN, {""}, {"The new contract sparked a labor dispute."}),
	MyVoca("labor", VERB, {""}, { "Construction workers labor outdoors, often regardless of weather conditions." }),
	MyVoca("device", NOUN, { "gadget" }, { "The new device was tested for possible defects." }),
	MyVoca("deal", VERB, { "handle" }, { "The problem will be dealt with immediately." }),
	MyVoca("deal", NOUN, { " " }, { "Fly-Age agency offers good deals on international flights." }),
	MyVoca("satisfaction", NOUN, { "content" }, { "We hope our assistance was to your satisfacion." }),
	MyVoca("critical", ADJ, { "important" }, { "Many customers were critical of the new wervices." }),

	/* 91 ~ 100 */
	MyVoca("assemble", VERB, { "build", "call together" }, { "The manager assembled everyone in the department for a meeting." }),
	MyVoca("utilize", VERB, { "use" }, { "The technicians utilized computer technology to improve processes." }),
	MyVoca("place", VERB, { "leave", "put" }, { "The factory supervisor has placed production operations on standby." }),
	MyVoca("material", NOUN, { "substance" }, { "The designers selected the material because of its durability." }),
	MyVoca("produce", ADJ, { "turn out" }, { "The new machinery produces 1,000 units per hour." }),
	MyVoca("feature", NOUN, { "characteristic" }, { "The latest dryer has several new features." }),
	MyVoca("feature", VERB, { " " }, { "The refrigerator model features high energy effciency." }),
	MyVoca("grant", VERB, { " " }, { "The patent for the handheld computer was granted on April 27." }),
	MyVoca("grant", NOUN, { "allowance" }, { "The research team will receive a government grant of up to $4,000." }),
	MyVoca("absolute", ADJ, { "complete", "utter" }, { "The latest technology keeps production costs to an absolute minimum." }),

	{ "-1", NOUN, { "" }, { "" } }, // end sentinel
};
